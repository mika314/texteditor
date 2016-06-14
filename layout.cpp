#include "layout.hpp"
#include "widget.hpp"
#include <limits>
#include <algorithm>

Layout::Layout(Style style):
  style_(style),
  left_(0),
  top_(0),
  width_(0),
  height_(0)
{
}

void Layout::addLayoutable(Layoutable *value)
{
  layoutablesList_.push_back(value);
  value->setParentLayout(this);
  resize(width_, height_);
}

void Layout::removeLayoutable(Layoutable *value)
{
  auto iter = std::find(begin(layoutablesList_), end(layoutablesList_), value);
  if (iter != end(layoutablesList_))
  {
    layoutablesList_.erase(iter);
    resize(width_, height_);
  }
}

void Layout::setLeft(int value)
{
  left_ = value;
}

void Layout::setTop(int value)
{
  top_ = value;
}

static std::vector<int> calculateDimensions(std::vector<std::pair<int, int>> minMax, int dimension)
{
  std::vector<int> result;
  std::vector<bool> untouchableList;
  for (size_t i = 0; i < minMax.size(); ++i)
  {
    result.push_back((i + 1) * dimension / minMax.size() - i * dimension / minMax.size());
    untouchableList.push_back(false);
  }
  auto touchableCount = minMax.size();
  auto remDim = dimension;
  auto resultIter = begin(result);
  auto untouchableListIter = begin(untouchableList);
  for (auto &l: minMax)
  {
    if (*resultIter < l.first)
    {
      *resultIter = l.first;
      *untouchableListIter = true;
      --touchableCount;
      remDim -= *resultIter;
    }
    else if (*resultIter > l.second)
    {
      *resultIter = l.second;
      *untouchableListIter = true;
      --touchableCount;
      remDim -= *resultIter;
    }
    ++resultIter;
    ++untouchableListIter;
  }
  untouchableListIter = begin(untouchableList);
  int i = 0;
  for (auto &dim: result)
  {
    if (!*untouchableListIter)
      dim = (i + 1) * remDim / touchableCount - i * remDim / touchableCount;
    ++i;
    ++untouchableListIter;
  }
  return result;
}

void Layout::resize(int width, int height)
{
  width_ = width;
  height_ = height;
  if (style_ == Vertical)
  {
    std::vector<std::pair<int, int> > minMax;
    for (auto &l: layoutablesList_)
      minMax.push_back(std::make_pair(l->minHeight(), l->maxHeight()));
    auto heights = calculateDimensions(minMax, height_);
    auto heightsIter = begin(heights);
    auto t = top_;
    for (auto &l: layoutablesList_)
    {
      l->setLeft(left_);
      l->setTop(t);
      l->resize(width, *heightsIter);
      t += *heightsIter;
      ++heightsIter;
    }
  }
  else // if style == Horizontal
  {
    std::vector<std::pair<int, int> > minMax;
    for (auto &l: layoutablesList_)
      minMax.push_back(std::make_pair(l->minWidth(), l->maxWidth()));
    auto widths = calculateDimensions(minMax, width_);
    auto widthsIter = begin(widths);
    auto lt = left_;
    for (auto &l: layoutablesList_)
    {
      l->setLeft(lt);
      l->setTop(top_);
      l->resize(*widthsIter, height_);
      lt += *widthsIter;
      ++widthsIter;
    }
  }
}

int Layout::maxHeight() const
{
  return std::numeric_limits<int>::max();
}

int Layout::minHeight() const
{
  return 0;
}

int Layout::maxWidth() const
{
  return std::numeric_limits<int>::max();
}

int Layout::minWidth() const
{
  return 0;
}

void Layout::setStyle(Style value)
{
  if (style_ != value)
  {
    style_ = value;
    resize(width_, height_);
  }
}

std::vector<Layoutable *> Layout::children() const
{
  return layoutablesList_;
}
