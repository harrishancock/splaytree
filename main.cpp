#include "symbol_table.hpp"

#include <vector>

const std::string open_scope_token { "{" };
const std::string close_scope_token { "}" };

std::vector<std::string> testfile {
    open_scope_token,
        "a", "ab", "ba", "abbab", "ab", "ba",
        open_scope_token,
            "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ",
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
            "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ",
        close_scope_token,
        open_scope_token,
            "a",
            "ab",
            "ba",
            "AB",
            "BA",
            "AB",
            "BA",
            "ab",
            open_scope_token,
                "a",
                "ab",
                "AB",
            close_scope_token,
            "a",
            "ab",
            "ba",
            "AB",
            "BA",
            "AABB",
        close_scope_token,
        "a",
        "ba",
        "cd",
        "dc",
    close_scope_token,
};

int main () {
    symbol_table_scope_manager symtab;

    for (auto lexeme : testfile) {
        if (open_scope_token == lexeme) {
            symtab.open_scope();
        }
        else if (close_scope_token == lexeme) {
            symtab.close_scope();
        }
        else {
            auto pair = symtab.insert(lexeme);
            ++pair.first->second.reference_count;
        }
    }

    symtab.display();
}
