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
    Screen screen_;
    Tabs tabs_;
    StatusBar statusBar_;
    Layout layout_;
    void openFile(OpenDialog *, std::string);
    void saveAs(SaveDialog *sender, TextFile *textFile, std::string fileName);
    void saveAndClose(SaveDialog *sender, TextFile *textFile, std::string fileName);
    void closeActiveTextBuffer(Dialog::Answer);
    void save();
};
