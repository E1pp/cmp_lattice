include(FetchContent)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)
# set(FETCHCONTENT_FULLY_DISCONNECTED ON)

# set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Libraries

# --------------------------------------------------------------------

# fmt with println

project_log("FetchContent: fmt")

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 87c066a35b7cc70bb7d438a543c8b49b577e61f4
)
FetchContent_MakeAvailable(fmt)

# --------------------------------------------------------------------

# Unique Function

project_log("FetchContent: function2")

FetchContent_Declare(
        function2
        GIT_REPOSITORY https://github.com/Naios/function2.git
        GIT_TAG 4.2.2
)
FetchContent_MakeAvailable(function2)

# --------------------------------------------------------------------

# tl::expected

project_log("FetchContent: expected")

FetchContent_Declare(
        expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG cmake_love
)
FetchContent_MakeAvailable(expected)

# --------------------------------------------------------------------

FetchContent_Declare(
    armadillo
    GIT_REPOSITORY https://gitlab.com/conradsnicta/armadillo-code
    GIT_TAG f82a749b4b8f24ef753cd3bd9e7008d9cee3a07a
)
FetchContent_MakeAvailable(armadillo)

# --------------------------------------------------------------------

project_log("FetchContent: weave")

FetchContent_Declare(
        weave
        GIT_REPOSITORY https://E1pp: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@github.com/E1pp/weave.git
        GIT_TAG main
)
FetchContent_MakeAvailable(weave)

# --------------------------------------------------------------------
