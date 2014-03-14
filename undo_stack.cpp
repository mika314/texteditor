#include "undo_stack.hpp"

BaseUndoCommand::~BaseUndoCommand()
{}


UndoStack::~UndoStack()
{
    for (auto c: undoStack_)
        delete c;
    for (auto c: redoStack_)
        delete c;
}


void UndoStack::undo(Coord &cursor)
{
    if (undoStack_.empty())
        return;
    auto command = undoStack_.back();
    undoStack_.pop_back();
    redoStack_.push_back(command);
    command->undo(cursor);
}

void UndoStack::redo(Coord &cursor)
{
    if (redoStack_.empty())
        return;
    auto command = redoStack_.back();
    redoStack_.pop_back();
    undoStack_.push_back(command);
    command->redo(cursor);
}

