#include <my_lisp/contract_helpers.hpp>


void ASSERT(bool                  condition,
            std::string_view      message,
            std::ostream         &stream,
 [[maybe_unused]] std::source_location  sloc)
{
    if ( !condition )
    {
        stream << "[Assertion Violation] " << message << std::endl;
        assert(false);
    }
}

void PRECONDITION(bool                  condition,
                  std::string_view      message,
                  std::ostream         &stream,
 [[maybe_unused]] std::source_location  sloc)
{
    if ( !condition )
    {
        stream << "[Precondition Violation] " << message << std::endl;
        assert(false);
    }
}

void POSTCONDITION(bool                  condition,
                   std::string_view      message,
                   std::ostream         &stream,
  [[maybe_unused]] std::source_location  sloc)
{
    if ( !condition )
    {
        stream << "[Postcondition Violation] " << message << std::endl;
        assert(false);
    }
}
