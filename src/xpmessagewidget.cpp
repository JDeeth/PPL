// Copyright (C) 2008,2009,2010 by Philipp Muenzel. All rights reserved
// Released under the terms of the GNU General Public License version 2 or later
// as published by the Free Software Foundation, Inc.

#include <cstdlib>

#include "xpmessagewidget.h"

using namespace PPL;

XPMessageWidget::XPMessageWidget(int width, int height, const std::string& title, const std::string& message, bool quit):
        m_is_displayed(false),
        m_xp_width("sim/graphics/view/window_width"),
        m_xp_height("sim/graphics/view/window_height")
{
    try
    {
        setWidth(width);
        setHeight(height);
    } catch (BoxOutOfBoundsException& ex)
    {
        Log() << Log::Error << "Cannot create widget with dimensions: " << ex.what() << Log::endl;
        try {
            setWidth(m_xp_width - 100);
            setHeight(m_xp_height - 100);
        } catch(BoxOutOfBoundsException&)
        {
            Log() << Log::Info << "Something is seriously fucked up with window mngmt ..." << Log::endl;
            std::abort();
        }
    }

    setTitle(title);
    setMessage(message);
    quitSimOnConfirm(quit);
    display();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

XPMessageWidget::~XPMessageWidget()
{
    XPSetWidgetProperty(m_box_widget, xpProperty_Object, 0);
    XPDestroyWidget(m_box_widget,1);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::setWidth(int width)
{
    if (width > m_xp_width)
        throw (BoxOutOfBoundsException("Requested window width greater than X-Plane window"));
    m_left = (m_xp_width - width)/2;
    m_right = (m_xp_width + width)/2;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::setHeight(int height)
{
    if (height > m_xp_height)
        throw (BoxOutOfBoundsException("Requested window height greater than X-Plane window"));
    m_top = (m_xp_height + height)/2;
    m_bottom = (m_xp_width - height)/2;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::setTitle(const std::string &title)
{
    m_title = title;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::setMessage(const std::string &message)
{
    m_message = message;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::quitSimOnConfirm(bool quit)
{
    m_quit_on_confirm = quit;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::display()
{
    createSurroundingBox();
    createInnerScreen();
    // break up string in portions
    //!< @todo Measure actual string length with 2.0 function and decide according to window width
    const unsigned int max_chars = 70;
    std::list<std::string> list_of_words;
    splitStr(list_of_words, m_message, " ", false);

    std::list<std::string> list_of_sequences;
    std::string seq;
    while (!list_of_words.empty())
    {
        if (seq.size() + list_of_words.front().size() < max_chars)
        {
            // Adding the current word to the sequence doesn't exceed max length. Append it and remove it from words
            seq.append(list_of_words.front()).append(" ");
            list_of_words.pop_front();
        } else {
            // The string would grow too large. save its actual contents and clear the temporary string
            list_of_sequences.push_back(seq);
            seq.clear();
        }
    }
    if (!seq.empty())
        list_of_sequences.push_back(seq);
    // for each sequence of maximum length, create a caption inside the screen widget
    m_caption_widgets_list.reserve(list_of_sequences.size());
    for (int i=0 ; !list_of_sequences.empty() ; i++)
    {
        m_caption_widgets_list.push_back( XPCreateWidget(m_left + 10,
                                                         m_top - 30 - i * 12,
                                                         m_right - 10,
                                                         m_top - 40 - i * 12,
                                                         1,
                                                         list_of_sequences.front().c_str(),
                                                         0,
                                                         m_screen_widget,
                                                         xpWidgetClass_Caption)
                                          );
        list_of_sequences.pop_front();
    }

    XPSetWidgetProperty(m_screen_widget, xpProperty_SubWindowType, xpSubWindowStyle_SubWindow);
    XPSetWidgetProperty(m_box_widget, xpProperty_Object, reinterpret_cast<long>(this));
    XPAddWidgetCallback(m_box_widget, widgetCallback);
    m_is_displayed = true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool XPMessageWidget::isDisplayed()
{
    return m_is_displayed;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int XPMessageWidget::processMessages(XPWidgetMessage inMessage, long, long)
{
    if (inMessage == xpMessage_CloseButtonPushed)
    {
        if (m_quit_on_confirm)
        {
            //Log::finish();
            std::exit(1);
        } else
        {
            XPDestroyWidget(m_box_widget, 1);
            m_is_displayed = false;
        }
        return 1;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int XPMessageWidget::widgetCallback(XPWidgetMessage inMessage, XPWidgetID inWidget, long param1, long param2)
{
    XPMessageWidget* widget = reinterpret_cast<XPMessageWidget *>(XPGetWidgetProperty(inWidget, xpProperty_Object, NULL));
    if (widget)
    {
        return widget->processMessages(inMessage, param1, param2);
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int XPMessageWidget::splitStr(std::list<std::string>& L, const std::string& seq, const std::string& _1cdelim, bool _removews )
{
    typedef std::string::size_type ST;
    std::string delims = _1cdelim;
    std::string STR;
    if(delims.empty()) delims = "\n\r";
    if(_removews) delims += " ";

    ST pos=0, LEN = seq.size();
    while(pos < LEN )
    {
        STR=""; // Init/clear the STR token buffer

        // remove any delimiters including optional (white)spaces
        while( (delims.find(seq[pos]) != std::string::npos) && (pos < LEN) ) ++pos;

        // leave if @eos
        if(pos==LEN) return L.size();
        // Save token data
        while( (delims.find(seq[pos]) == std::string::npos) && (pos < LEN) ) STR += seq[pos++];
        // put valid STR buffer into the supplied list
        if( ! STR.empty() ) L.push_back(STR);
    }
    return L.size();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::createSurroundingBox()
{
    m_box_widget = XPCreateWidget(m_left,
                                  m_top,
                                  m_right,
                                  m_bottom,
                                  1,
                                  m_title.c_str(),
                                  1,
                                  0,
                                  xpWidgetClass_MainWindow);
    XPSetWidgetProperty(m_box_widget, xpProperty_MainWindowHasCloseBoxes, 1);
    XPSetWidgetProperty(m_box_widget, xpProperty_MainWindowType, xpMainWindowStyle_MainWindow);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void XPMessageWidget::createInnerScreen()
{
    m_screen_widget = XPCreateWidget(m_left + 5,
                                     m_top - 25,
                                     m_right - 5,
                                     m_bottom + 5,
                                     1,
                                     "",
                                     0,
                                     m_box_widget,
                                     xpWidgetClass_SubWindow);
}
