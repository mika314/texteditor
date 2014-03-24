#include "main_window.hpp"
#include "key_event.hpp"
#include "open_dialog.hpp"
#include "save_dialog.hpp"
#include "text_file.hpp"

MainWindow::MainWindow(Widget *parent):
    Widget(parent),
    screen_(this),
    tabs_(this),
    statusBar_(this),
    layout_(Layout::Vertical)
{
    connect(SIGNAL(&tabs_, setTextBuffer), SLOT(&screen_, setTextBuffer));
    screen_.setStatusBar(&statusBar_);
    setLayout(&layout_);
    layout_.addWidget(&tabs_);
    layout_.addWidget(&screen_);
    layout_.addWidget(&statusBar_);
    screen_.setFocus();
}

bool MainWindow::keyPressEvent(KeyEvent &e)
{
    bool result1 = true;
    switch (e.modifiers()) 
    {
    case KeyEvent::MLCtrl:
    case KeyEvent::MRCtrl:
        switch (e.key())
        {
        case KeyEvent::KO:
            {
                auto openDialog = new OpenDialog(&screen_);
                connect(SIGNAL(openDialog, openFile), SLOT(this, openFile));
                tabs_.addTextBuffer(openDialog);
                break;
            }
        case KeyEvent::KS:
            if (auto textFile = dynamic_cast<TextFile *>(screen_.textBuffer()))
            {
                if (textFile->fileName().empty())
                {
                    auto saveDialog = new SaveDialog(&screen_, textFile);
                    tabs_.addTextBuffer(saveDialog);
                    connect(SIGNAL(saveDialog, saveAs), SLOT(this, saveAs));
                }
                else
                    textFile->save();
            }
            break;
        case KeyEvent::KN:
            tabs_.addTextBuffer(new TextFile);
            break;
        case KeyEvent::KW:
            tabs_.closeActiveTextBuffer();
            break;
        case KeyEvent::KLeft:
            tabs_.switchToPrevTextBuffer();
            break;
        case KeyEvent::KRight:
            tabs_.switchToNextTextBuffer();
            break;
        default:
            result1 = false;
        }
    default:
        result1 = false;
    }

    bool result2 = true;
    if ((e.modifiers() & KeyEvent::MCtrl) != 0 && (e.modifiers() & KeyEvent::MShift) != 0)
    {
        switch (e.key())
        {
        case KeyEvent::KLeft:
            tabs_.moveTextBufferLeft();
            break;
        case KeyEvent::KRight:
            tabs_.moveTextBufferRight();
            break;
        default:
            result2 = false;
        }
    }
    else
        result2 = false;

    return result1 || result2;
}

void MainWindow::openFile(OpenDialog *sender, std::string fileName)
{
    tabs_.closeTextBuffer(sender);
    tabs_.addTextBuffer(new TextFile(fileName));
}

void MainWindow::saveAs(SaveDialog *sender, TextFile *textFile, std::string fileName)
{
    tabs_.closeTextBuffer(sender);
    tabs_.setActiveTextBuffer(textFile);
    textFile->saveAs(fileName);
}
