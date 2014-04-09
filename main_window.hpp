#pragma once
#include "tabs.hpp"
#include "screen.hpp"
#include "status_bar.hpp"
#include "layout.hpp"
#include "widget.hpp"
#include "dialog.hpp"
#include <string>

class OpenDialog;
class SaveDialog;
class TextFile;

class MainWindow: public Widget
{
public:
    MainWindow(Widget *parent = nullptr);
protected:
    virtual bool keyPressEvent(KeyEvent &);
private:
    Screen *activeScreen_;
    Layout layout_;
    Tabs tabs_;
    StatusBar statusBar_;
    Layout screenLayout_;
    void openFile(OpenDialog *, std::string);
    void saveAs(SaveDialog *sender, TextFile *textFile, std::string fileName);
    void saveAndClose(SaveDialog *sender, TextFile *textFile, std::string fileName);
    void closeActiveTextBuffer(Dialog::Answer);
    void save();
    void wholeScreen();
    void split(Layout::Style);
    void switchToPrevScreen();
    void switchToNextScreen();
    void setTextBuffer(BaseTextBuffer *);
    void deleteTextBuffer(BaseTextBuffer *);
};
