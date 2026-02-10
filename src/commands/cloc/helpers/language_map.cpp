#include "commands/cloc/helpers/language_map.h"

#include <string>
#include <unordered_map>

namespace fima {

namespace cloc {

namespace helpers {

// clang-format off
std::unordered_map<std::string, std::string> language_map = {
    // c-like
    { ".cpp", "c_like" },
    { ".cxx", "c_like" },
    { ".cc", "c_like" },
    { ".h", "c_like" },
    { ".hpp", "c_like" },
    { ".hxx", "c_like" },
    { ".inl", "c_like" },
    { ".rs", "c_like" },
    { ".cs", "c_like" },
    { ".js", "c_like" },
    { ".ts", "c_like" },
    { ".java", "c_like" },
    { ".m", "c_like" },
    { ".php", "c_like" },
    { ".dart", "c_like" },
    { ".go", "c_like" },
    { ".swift", "c_like" },
    { ".kt", "c_like" },
    { ".scala", "c_like" },
    { ".hx", "c_like" },
    { ".hxml", "c_like" },

    // lisp-like
    { ".lisp", "lisp_like" },
    { ".lsp", "lisp_like" },
    { ".cl", "lisp_like" },
    { ".scm", "lisp_like" },
    { ".rkt", "lisp_like" },
    { ".clj", "lisp_like" },
    { ".cljs", "lisp_like" },
    { ".cljc", "lisp_like" },
    { ".edn", "lisp_like" },
    { ".el", "lisp_like" },
    { ".ss", "lisp_like" },
    { ".hy", "lisp_like" },
    { ".janet", "lisp_like" },
    { ".fnl", "lisp_like" },
    { ".asm", "lisp_like" },
    { ".s", "lisp_like" },

    // shell-like
    { ".sh", "shell_like" },
    { ".zsh", "shell_like" },
    { ".fish", "shell_like" },
    { ".dash", "shell_like" },
    { ".ksh", "shell_like" },
    { ".ash", "shell_like" },
    { ".tcsh", "shell_like" },
    { ".csh", "shell_like" },
    { ".rb", "shell_like" },
    { ".pl", "shell_like" },
    { ".pm", "shell_like" },
    { ".raku", "shell_like" },
    { ".rakumod", "shell_like" },
    { ".p6", "shell_like" },
    { ".jl", "shell_like" },
    { ".nim", "shell_like" },
    { ".cr", "shell_like" },
    { ".yml", "shell_like" },
    { ".yaml", "shell_like" },
    { ".nim", "shell_like" },
    { ".tf", "shell_like" },
    { ".nix", "shell_like" },
    { ".sls", "shell_like" },

    // "special" languages
    { ".py", "python" }
};
// clang-format on

[[nodiscard]] std::string
get_language_family(std::string extension)
{
    return language_map.at(extension);
}

}

}

}
