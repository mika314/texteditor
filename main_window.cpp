#include "main_window.hpp"
#include "key_event.hpp"
#include "open_dialog.hpp"
#include "save_dialog.hpp"
#include "text_file.hpp"
#include "dialog.hpp"
#include "application.hpp"

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
            save();
            break;
        case KeyEvent::KN:
            tabs_.addTextBuffer(new TextFile);
            break;
        case KeyEvent::KW:
            if (dynamic_cast<TextFile *>(tabs_.activeTextBuffer()) && tabs_.activeTextBuffer()->isModified())
            {
                if (!statusBar_.textBuffer())
                {
                    auto d = new Dialog(L"The file is modified. Do you want to save it before closing?");
                    statusBar_.setTextBuffer(d);
                    connect(SIGNAL(d, result), SLOT(this, closeActiveTextBuffer));
                }
            }
            else
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

void MainWindow::saveAndClose(SaveDialog *sender, TextFile *textFile, std::string fileName)
{
    tabs_.closeTextBuffer(sender);
    tabs_.closeTextBuffer(textFile);
    textFile->saveAs(fileName);
}

void MainWindow::closeActiveTextBuffer(Dialog::Answer value)
{
    auto d = statusBar_.textBuffer();
    Application::instance()->queueDelete(d);
    statusBar_.setTextBuffer(nullptr);
    if (auto textFile = dynamic_cast<TextFile *>(screen_.textBuffer()))
    {
        switch (value)
        {
        case Dialog::Yes:
            if (textFile->fileName().empty())
            {
                auto saveDialog = new SaveDialog(&screen_, textFile);
                tabs_.addTextBuffer(saveDialog);
                screen_.setCursor(0, 1);
                connect(SIGNAL(saveDialog, saveAs), SLOT(this, saveAndClose));
            }
            else
            {
                textFile->save();
                tabs_.closeActiveTextBuffer();
            }
            break;
        case Dialog::No:
            tabs_.closeActiveTextBuffer();
            break;
        case Dialog::Cancel:
            break;
            
        }
        tabs_.update();
    }
}

void MainWindow::save()
{
    if (auto textFile = dynamic_cast<TextFile *>(screen_.textBuffer()))
    {
        if (textFile->fileName().empty())
        {
            auto saveDialog = new SaveDialog(&screen_, textFile);
            tabs_.addTextBuffer(saveDialog);
            screen_.setCursor(0, 1);
            connect(SIGNAL(saveDialog, saveAs), SLOT(this, saveAs));
        }
        else
            textFile->save();
        tabs_.update();
    }
}
