#include "main_window.hpp"
#include "key_event.hpp"
#include "open_dialog.hpp"
#include "save_dialog.hpp"
#include "text_file.hpp"
#include "dialog.hpp"
#include "application.hpp"

MainWindow::MainWindow(Widget *parent):
    Widget(parent),
    activeScreen_(nullptr),
    layout_(Layout::Vertical),
    tabs_(this),
    statusBar_(this),
    screenLayout_(Layout::Vertical)
{
    activeScreen_ = new Screen(this);
    connect(SIGNAL(&tabs_, setTextBuffer), SLOT(activeScreen_, setTextBuffer));
    activeScreen_->setStatusBar(&statusBar_);
    setLayout(&layout_);
    layout_.addLayoutable(&tabs_);
    screenLayout_.addLayoutable(activeScreen_);
    layout_.addLayoutable(&screenLayout_);
    layout_.addLayoutable(&statusBar_);
    activeScreen_->setFocus();
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
        case KeyEvent::K2:
            wholeScreen();
            break;
        case KeyEvent::K3:
            split(Layout::Vertical);
            break;
        case KeyEvent::K4:
            split(Layout::Horizontal);
            break;
        case KeyEvent::KO:
            {
                auto openDialog = new OpenDialog(activeScreen_);
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
        break;
    case KeyEvent::MLAlt:
    case KeyEvent::MRAlt:
        switch (e.key())
        {
        case KeyEvent::KLeft:
            // TODO switchToPrevScreen();
            break;
        case KeyEvent::KRight:
            // TODO switchToNextScreen();
            break;
        default:
            result1 = false;
        }
        break;
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
    if (auto textFile = dynamic_cast<TextFile *>(activeScreen_->textBuffer()))
    {
        switch (value)
        {
        case Dialog::Yes:
            if (textFile->fileName().empty())
            {
                auto saveDialog = new SaveDialog(activeScreen_, textFile);
                tabs_.addTextBuffer(saveDialog);
                activeScreen_->setCursor(0, 1);
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
    if (auto textFile = dynamic_cast<TextFile *>(activeScreen_->textBuffer()))
    {
        if (textFile->fileName().empty())
        {
            auto saveDialog = new SaveDialog(activeScreen_, textFile);
            tabs_.addTextBuffer(saveDialog);
            activeScreen_->setCursor(0, 1);
            connect(SIGNAL(saveDialog, saveAs), SLOT(this, saveAs));
        }
        else
            textFile->save();
        tabs_.update();
    }
}

static void markForDeleteRecursively(Layoutable *value)
{
    if (auto layout = dynamic_cast<Layout *>(value))
    {
        auto children = layout->children();
        for (auto child: children)
        {
            markForDeleteRecursively(child);
            Application::instance()->queueDelete(child);
        }
    }
}

void MainWindow::wholeScreen()
{
    markForDeleteRecursively(&screenLayout_);
    auto screen = activeScreen_;
    activeScreen_ = new Screen(this);
    connect(SIGNAL(&tabs_, setTextBuffer), SLOT(activeScreen_, setTextBuffer));
    activeScreen_->setStatusBar(&statusBar_);
    screenLayout_.addLayoutable(activeScreen_);
    activeScreen_->setTextBuffer(screen->textBuffer());
    activeScreen_->setFocus();
}

void MainWindow::split(Layout::Style style)
{
    auto layout = activeScreen_->parentLayout();
    layout->setStyle(style);
    auto l1 = new Layout(Layout::Vertical);
    auto l2 = new Layout(Layout::Vertical);
    layout->removeLayoutable(activeScreen_);
    layout->addLayoutable(l1);
    layout->addLayoutable(l2);
    l1->addLayoutable(activeScreen_);
    auto s2 = new Screen(this);
    connect(SIGNAL(&tabs_, setTextBuffer), SLOT(s2, setTextBuffer));
    s2->setStatusBar(&statusBar_);
    s2->setTextBuffer(activeScreen_->textBuffer());
    l2->addLayoutable(s2);
}

