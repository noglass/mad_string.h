#ifndef MAD__STRING
#define MAD__STRING

#include <string>
#include <vector>
#include <stdexcept>

namespace mad
{
    struct string : public std::string
    {
        using std::string::string;
        string(std::string&& str) : std::string(std::move(str)) {}
        /* mad::string::slice
            Just like std::string::substr() except both arguments are actual positions in the string
            Example:
                mad::string("hello world!").slice(6,10) == mad::string("world");
        */
        inline string slice(size_t pos1, size_t pos2)
        {
            return this->substr(pos1,pos2+1-pos1);
        }
        /* mad::string::trim
            Remove all leading, trailing, and repeated instances of str.
            Example:
                mad::string(" hello      w or ld!   ").trim(" ") == mad::string(" hello w or ld! ");
        */
        string& trim(const std::string& str)
        {
            while (this->find(str) == 0)
                this->erase(0,str.size());
            size_t pos;
            for (std::string tmp = str + str;(pos = this->find(tmp))+1;this->erase(pos,str.size()));
            for (;this->compare((pos = this->size()-str.size()),std::string::npos,str) == 0;this->erase(pos));
            return *this;
        }
        /* mad::string::trim_all_of
            Same as mad::string::trim except any is a string of individual characters to be trimmed.
                Consecutive characters much match.
            Example:
                mad::string("hello ; ;; ; world ;").trim_all_of(" ;") == mad::string("hello ; ; ; world");
        */
        string& trim_all_of(const std::string& any)
        {
            bool front = true;
            for (auto c = this->begin();c != this->end();)
            {
                if ((any.find(*c)+1) && ((front) || (c+1 == this->end()) || (*c == *(c+1))))
                    c = this->erase(c);
                else
                {
                    ++c;
                    front = false;
                }
            }
            return *this;
        }
        /* mad::string::trim_any_of
            Same as mad::string::trim_all_of except consecutive chars do not need to match.
            Example:
                mad::string("hello ; ;; ; world ;").trim_any_of(" ;") == mad::string("hello world");
        */
        string& trim_any_of(const std::string& any)
        {
            bool front = true;
            for (auto c = this->begin();c != this->end();)
            {
                if ((any.find(*c)+1) && ((front) || (c+1 == this->end()) || (any.find(*(c+1))+1)))
                    c = this->erase(c);
                else
                {
                    ++c;
                    front = false;
                }
            }
            return *this;
        }
        /* mad::string::tokenize
            Fill the internal array of token positions based on a delimiter.
                You can also specify optional characters that can be wrapped around parts of the
                    string to ensure they become a single token, ie: "quoted strings".
            Example:
                mad::string("this is a string of arguments").tokenize(" ");
                // Split the string up at each instance of a space.
        */
        size_t tokenize(const std::string& delimiter, const std::string& quote_chars = "")
        {
            tokens.clear();
            char c;
            for (size_t i = 0, s = this->size(), last = 0, temp = 0;i < s;)
            {
                if ((quote_chars.find(c = this->at(i))+1) && ((temp = this->find(c,i+1))+1))
                    i = temp;
                if ((i+1 == s) || (this->compare(i,delimiter.size(),delimiter) == 0))
                {
                    temp = i;
                    if ((i > 0) && (i+1 < s))
                        --temp;
                    if ((last != temp) && (temp > 0) && (quote_chars.find(this->at(temp))+1) && (quote_chars.find(this->at(last))+1))
                    {
                        ++last;
                        --temp;
                    }
                    tokens.emplace_back(last,temp);
                    last = (i+=delimiter.size());
                }
                else
                    ++i;
            }
            return tokens.size();
        }
        /* mad::string::tokenize_any
            Same as mad::string::tokenize except delimiters is a string of individual characters to
                be used as single character delimiters.
            Example:
                mad::string("this\nis a\tstring of\rarguments").tokenize_any("\n\r\t ");
                // Produces the exact same result as the example from mad::string::tokenize
        */
        size_t tokenize_any(const std::string& delimiters, const std::string& quote_chars = "")
        {
            tokens.clear();
            char c;
            for (size_t i = 0, s = this->size(), last = 0, temp = 0;i < s;++i)
            {
                if ((quote_chars.find(c = this->at(i))+1) && ((temp = this->find(c,i+1))+1))
                    i = temp;
                if ((i+1 == s) || (delimiters.find(c)+1))
                {
                    temp = i;
                    if ((i > 0) && (i+1 < s))
                        --temp;
                    if ((last != temp) && (temp > 0) && (quote_chars.find(this->at(temp))+1) && (quote_chars.find(this->at(last))+1))
                    {
                        ++last;
                        --temp;
                    }
                    tokens.emplace_back(last,temp);
                    last = i+1;
                }
            }
            return tokens.size();
        }
        /* mad::string::split
            Convenience function for automatically calling mad::string::tokenize and returning a
                vector filled with copies of substrings for each token.
            Example:
                mad::string("this is a string of arguments").tokenize(" ") == {"this","is","a","string","of","arguments"};
        */
        std::vector<string> split(const std::string& delimiter = "", const std::string& quote_chars = "")
        {
            if (delimiter.size())
                tokenize(delimiter,quote_chars);
            std::vector<string> v;
            for (auto& p : tokens)
                v.emplace_back(slice(p.first,p.second));
            return std::move(v);
        }
        /* mad::string::split_any
            Convenience function for automatically calling mad::string::tokenize_any and returning a
                vector filled with copies of substrings for each token.
            Example:
                mad::string("this\nis a\tstring of\rarguments").tokenize(" ") == {"this","is","a","string","of","arguments"};
        */
        std::vector<string> split_any(const std::string& delimiters = "", const std::string& quote_chars = "")
        {
            if (delimiters.size())
                tokenize_any(delimiters,quote_chars);
            std::vector<string> v;
            for (auto& p : tokens)
                v.emplace_back(slice(p.first,p.second));
            return std::move(v);
        }
        /* mad::string::get_token
            After calling mad::string::tokenize or mad::string::tokenize_any, this function will
                return a copy of the substring matching the token.
            Example:
                mad::string str = "this is a string of arguments";
                str.tokenize(" ");
                str.get_token(3) == mad::string("string");
        */
        string get_token(size_t token)
        {
            if (token >= tokens.size())
                throw std::out_of_range("mad::string::get_token(): " + std::to_string(token) + " Exceeds token size, which is " + std::to_string(tokens.size()) + '.');
            auto pos = tokens.at(token);
            return slice(pos.first,pos.second);
        }
        /* mad::string::get_token_pos
            Get a pair of positions marking where the token starts and ends (not length!)
                This is the most performant method of working with the tokenized list as no copies
                    are directly made.
            Example:
                mad::string str = "this is a string of arguments";
                str.tokenize(" ");
                str.get_token_pos(3) == std::pair<size_t,size_t>(10,15);
        */
        std::pair<size_t,size_t> get_token_pos(size_t token)
        {
            if (token >= tokens.size())
                throw std::out_of_range("mad::string::get_token_pos(): " + std::to_string(token) + " Exceeds token size, which is " + std::to_string(tokens.size()) + '.');
            return tokens.at(token);
        }
        /* mad::string::token_size
            Returns the total number of tokens found after tokenizing.
            Example:
                mad::string str = "this is a string of arguments";
                str.tokenize(" ");
                str.token_size() == 6;
        */
        inline size_t token_size() { return tokens.size(); }
        private:
            std::vector<std::pair<size_t,size_t>> tokens;
    };
}

#endif

