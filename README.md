# cppsimpleuri

A modern C++ uri & query parser. 

## Status

![Ubuntu](https://github.com/salzaverde/cppsimpleuri/workflows/Ubuntu/badge.svg)
![MacOS](https://github.com/salzaverde/cppsimpleuri/workflows/MacOS/badge.svg)
![Windows](https://github.com/salzaverde/cppsimpleuri/workflows/Windows/badge.svg)
[![codecov](https://codecov.io/gh/salzaverde/cppsimpleuri/branch/main/graph/badge.svg?token=K68N4BEVVS)](https://codecov.io/gh/salzaverde/cppsimpleuri)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/13cc4387adce4ebb9d24a808f63bd430)](https://app.codacy.com/gh/salzaverde/cppsimpleuri?utm_source=github.com&utm_medium=referral&utm_content=salzaverde/cppsimpleuri&utm_campaign=Badge_Grade_Settings)

## Features
-   RFC3986 conform
-   Includes Query Parser & Builder
-   Easy to use & understand
-   Easy to maintain & extend through unique factory pattern

## Tech
-   CMake
-   GoogleTest - 100% code coverage
-   C++20

## Integration
Include cppsimpleurilib in your cmake project using fetchContent:
```cpp
# CMakeLists.txt
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

auto myRawUri = std::string("https://myHomepage.com:1176/my/music?added=last#artist=rhcp");
auto uri = URI::parse(myRawUri);
    
//Access the uri's elements
auto scheme = uri->getScheme();
auto host = uri->getHost();
auto port = uri->getPort();
auto path = uri->getPath();
auto query = uri->getQuery();
auto fragment = uri->getFragment();
```

### Query Parsing
```cpp
#include <salzaverde/query.h>
using namespace salzaverde;

auto queryString = "artistfilter=rhcp&sortby=name";
auto query = Query::parse(queryString);

//Get value for key
auto paramValue = query->get("artistfilter");

//Modify
query->erase("artistfilter");
query->set("sortby", "duration");

//List keys
auto keys = query->listKeys();
```

### Query Building
```cpp
#include <salzaverde/query.h>
using namespace salzaverde;

//Step by step
std::map<std::string, std::string> parameters;
parameters.emplace("artistfilter", "rhcp");
parameters.emplace("sortby", "name");
auto query = Query::build(parameters);
	
//In place
auto query = Query::build({
    {"artistfilter", "rhcp"},
    {"sortby", "name"}
});
```
