#include "text_input_event.hpp"

TextInputEvent::TextInputEvent(std::wstring text):
    text_{text}
{}

std::wstring TextInputEvent::text() const
{
    return text_;
}
