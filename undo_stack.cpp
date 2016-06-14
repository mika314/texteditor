#include "undo_stack.hpp"

BaseUndoCommand::~BaseUndoCommand()
{}

UndoStack::UndoStack():
  originalState_(0)
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

bool UndoStack::canUndo() const
{
  return !undoStack_.empty();
}

bool UndoStack::canRedo() const
{
  return !redoStack_.empty();
}

void UndoStack::clean()
{
  for (auto c: undoStack_)
    delete c;
  undoStack_.clear();
  for (auto c: redoStack_)
    delete c;
  redoStack_.clear();
}

bool UndoStack::isModified() const
{
  return originalState_ != undoStack_.size();
}

void UndoStack::clearModified()
{
  originalState_ = undoStack_.size();
}
