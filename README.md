# cppsimpleuri

A modern C++ uri & query parser. 

## Status

![Ubuntu](https://github.com/salzaverde/cppsimpleuri/workflows/Ubuntu/badge.svg)
![MacOS](https://github.com/salzaverde/cppsimpleuri/workflows/MacOS/badge.svg)
![Windows](https://github.com/salzaverde/cppsimpleuri/workflows/Windows/badge.svg)
[![codecov](https://codecov.io/gh/salzaverde/cppsimpleuri/branch/main/graph/badge.svg?token=K68N4BEVVS)](https://codecov.io/gh/salzaverde/cppsimpleuri)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/13cc4387adce4ebb9d24a808f63bd430)](https://app.codacy.com/gh/salzaverde/cppsimpleuri?utm_source=github.com&utm_medium=referral&utm_content=salzaverde/cppsimpleuri&utm_campaign=Badge_Grade_Settings)

## Features
-   According to RFC3986
-   Includes Query Parser & Builder
-   Easy to use & understand
-   Comprehensively tested

## Requirements
-   CMake
-   C++11

## Integration
Include cppsimpleuri in your cmake project using fetchContent:
```cpp
# CMakeLists.txt

add_executable(my_target_name ...)

include(FetchContent)
FetchContent_Declare(
    cppsimpleuri
    GIT_REPOSITORY https://github.com/salzaverde/cppsimpleuri.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(cppsimpleuri)

target_link_libraries(my_target_name PRIVATE
    cppsimpleuri
)
```

## Examples

### URI Parsing
```cpp
#include <salzaverde/uri.h>
using namespace salzaverde;

auto myRawUri = std::string("https://salzaverde@github.io:1176/my/music?sort_by=title&before=2020#artist=rhcp");
auto uri = salzaverde::URI::parse(myRawUri);

std::cout << "Scheme:   "   << uri.scheme   << std::endl;
std::cout << "Userinfo: "   << uri.userinfo << std::endl;
std::cout << "Host:     "   << uri.host     << std::endl;
std::cout << "Port:     "   << uri.port     << std::endl;
std::cout << "Path:     "   << uri.path     << std::endl;
std::cout << "Query:    "   << uri.query    << std::endl;
std::cout << "Fragment: "   << uri.fragment << std::endl;

uri.dump(); //Outputs the raw uri
```

### Query Parsing
```cpp
#include <salzaverde/query.h>
using namespace salzaverde;

auto raw = "sort_by=title&before=2020"
auto query = salzaverde::Query::parse(raw);

query.parameters[sort_by] == "title"; //true
```

### Query Editing
```cpp
query.parameters["sort_by"] = "length";
query.dump(); //Query is now: "sort_by=length&before=2020"
```

### Query Construction
```cpp
#include <salzaverde/query.h>
using namespace salzaverde;

std::map<std::string, std::string> parameters;
parameters.emplace("artistfilter", "rhcp");
parameters.emplace("sortby", "name");
auto query1 = Query::build(parameters);

auto query2 = Query::build({
    {"artistfilter", "rhcp"},
    {"sortby", "name"}
});
```
