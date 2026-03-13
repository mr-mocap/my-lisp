#pragma once

#include <iostream>
#include <source_location>
#include <cassert>


#ifdef NDEBUG
#define ASSERT(condition) ((void)0)
#define ASSERT(condition, message) ((void)0)
#define ASSERT(condition, message, stream) ((void)0)

#define PRECONDITION(condition) ((void)0)
#define PRECONDITION(condition, message) ((void)0)
#define PRECONDITION(condition, message, stream) ((void)0)

#define POSTCONDITION(condition) ((void)0)
#define POSTCONDITION(condition, message) ((void)0)
#define POSTCONDITION(condition, message, stream) ((void)0)
#else
void ASSERT(bool                  condition,
            std::string_view      message   = "Assert failed",
            std::ostream         &stream    = std::cerr,
            std::source_location  sloc      = std::source_location::current());

template <typename T>
void ASSERT(std::shared_ptr<T>   &condition,
            std::string_view      message = "Assert failed",
            std::ostream         &stream = std::cerr,
            std::source_location  sloc = std::source_location::current())
{
    ASSERT(static_cast<bool>(condition), message, stream, sloc);
}

void PRECONDITION(bool                  condition,
                  std::string_view      message   = "Precondition failed",
                  std::ostream         &stream    = std::cerr,
                  std::source_location  sloc      = std::source_location::current());

template <typename T>
void PRECONDITION(std::shared_ptr<T>   &condition,
                  std::string_view      message = "Precondition failed",
                  std::ostream         &stream = std::cerr,
                  std::source_location  sloc = std::source_location::current())
{
    PRECONDITION(static_cast<bool>(condition), message, stream, sloc);
}

void POSTCONDITION(bool                  condition,
                   std::string_view      message   = "Postcondition failed",
                   std::ostream         &stream    = std::cerr,
                   std::source_location  sloc      = std::source_location::current());

template <typename T>
void POSTCONDITION(      std::shared_ptr<T>   &condition,
                   const std::string          &message = "Postcondition failed",
                         std::ostream         &stream  = std::cerr,
        [[maybe_unused]] std::source_location  sloc    = std::source_location::current())
{
    POSTCONDITION(static_cast<bool>(condition), static_cast<std::string_view>(message), stream, sloc);
}

template <typename T>
void POSTCONDITION(std::shared_ptr<T>   &condition,
                   std::string_view      message = "Postcondition failed",
                   std::ostream         &stream = std::cerr,
  [[maybe_unused]] std::source_location  sloc = std::source_location::current())
{
    POSTCONDITION(static_cast<bool>(condition), message, stream, sloc);
}
#endif
