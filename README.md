# mad_string.h
mad_string.h is a basic library adding helpful parsing methods to std::string commonly found in higher level languages.

## mad::string mad::string::slice(size_t pos1, size_t pos2)
Just like `std::string::substr()` except both arguments are actual positions in the string
```cpp
mad::string("hello world!").slice(6,10) == mad::string("world");
```

## mad::string& mad::string::trim(const std::string& str)
Remove all leading, trailing, and repeated instances of str.
```cpp
mad::string(" hello      w or ld!   ").trim(" ") == mad::string(" hello w or ld! ");
```

## mad::string& mad::string::trim_all_of(const std::string& any)
Same as `mad::string::trim` except any is a string of individual characters to be trimmed.  
  Consecutive characters much match.
```cpp
mad::string("hello ; ;; ; world ;").trim_all_of(" ;") == mad::string("hello ; ; ; world");
```

## mad::string& mad::string::trim_any_of(const std::string& any)
Same as `mad::string::trim_all_of` except consecutive chars do not need to match.
```cpp
mad::string("hello ; ;; ; world ;").trim_any_of(" ;") == mad::string("hello world");
```

## size_t mad::string::tokenize(const std::string& delimiter, const std::string& quote_chars = "")
Fill the internal array of token positions based on a delimiter.  
  You can also specify optional characters that can be wrapped around parts of the  
    string to ensure they become a single token, ie: "quoted strings".  
Returns number of tokens.
```cpp
mad::string("this is a string of arguments").tokenize(" ");
// Split the string up at each instance of a space.
```

## size_t mad::string::tokenize_any(const std::string& delimiters, const std::string& quote_chars = "")
Same as `mad::string::tokenize` except delimiters is a string of individual characters to  
  be used as single character delimiters.  
Returns number of tokens.
```cpp
mad::string("this\nis a\tstring of\rarguments").tokenize_any("\n\r\t ");
// Produces the exact same result as the example from mad::string::tokenize
```

## std::vector<mad::string> mad::string::split(const std::string& delimiter = "", const std::string& quote_chars = "")
Convenience function for automatically calling `mad::string::tokenize` and returning a  
  vector filled with copies of substrings for each token.
```cpp
mad::string("this is a string of arguments").tokenize(" ") == {"this","is","a","string","of","arguments"};
```

## std::vector<mad::string> mad::string::split_any(const std::string& delimiters = "", const std::string& quote_chars = "")
Convenience function for automatically calling `mad::string::tokenize_any` and returning a  
  vector filled with copies of substrings for each token.
```cpp
mad::string("this\nis a\tstring of\rarguments").tokenize(" ") == {"this","is","a","string","of","arguments"};
```

## mad::string mad::string::get_token(size_t token)
After calling `mad::string::tokenize` or `mad::string::tokenize_any`, this function will  
  return a copy of the substring matching the token.
```cpp
mad::string str = "this is a string of arguments";
str.tokenize(" ");
str.get_token(3) == mad::string("string");
```

## std::pair<size_t,size_t> mad::string::get_token_pos(size_t token)
Get a pair of positions marking where the token starts and ends (not length!)  
  This is the most performant method of working with the tokenized list as no copies  
    are directly made.  
```cpp
mad::string str = "this is a string of arguments";
str.tokenize(" ");
str.get_token_pos(3) == std::pair<size_t,size_t>(10,15);
```

## size_t mad::string::token_size()
Returns the total number of tokens found after tokenizing.
```cpp
mad::string str = "this is a string of arguments";
str.tokenize(" ");
str.token_size() == 6;
```
