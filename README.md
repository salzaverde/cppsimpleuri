[![Codacy Badge](https://app.codacy.com/project/badge/Grade/2f114b2127ba4a6fbe91a553a1a099de)](https://www.codacy.com/gh/salzaverde/cppsimpleuri/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=salzaverde/cppsimpleuri&amp;utm_campaign=Badge_Grade)

# cppsimpleuri

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/13cc4387adce4ebb9d24a808f63bd430)](https://app.codacy.com/gh/salzaverde/cppsimpleuri?utm_source=github.com&utm_medium=referral&utm_content=salzaverde/cppsimpleuri&utm_campaign=Badge_Grade_Settings)

A slim & simple uri parser.

## Features
-   Easy to integrate with CMake
-   Tested using GoogleTest
-   Open to change & extension

## Requirements
-   cmake
-   c++20

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

Build the examples from checkout:
```cpp
# Commandline
mkdir build
cmake -S. -B build -DSIMPLEURI_LIB_BUILD_EXAMPLES=On
cmake --build build/.
```

## Examples
```cpp
#include <salzaverde/uri.h>
#include <salzaverde/query.h>

int main(int argc, char *argv[]) {
    //Parsing
    auto myRawUri = std::string("https://myHomepage.com:1176/my/music?added=last#artist=rhcp");
    auto uri = salzaverde::URI::parse(myRawUri);
    
    //Access the uri's elements
    auto scheme = uri->getScheme();
    auto host = uri->getHost();
    auto port = uri->getPort();
    auto path = uri->getPath();
    auto queryString = uri->getQuery();
    auto fragment = uri->getFragment();
    
    //Query parsing
    auto query = salzaverde::Query::parse(queryString);
    query->set("sortby", "name");
    
    //Retrieve a query parameter value by key
    auto sortbyValue = query->get("sortby");
    
    //A value for this key might not exist
    if(! sortbyValue) {
        //Do some error handling
    }
       
    //Build a new query string in place
    auto newQuery = salzaverde::Query::build({
        {"list", "songs"},
        {"include", "rhcp"}
    });

    //Check if a key exists
    auto containsListKey = newQuery->contains("list");
    auto keys = query->listKeys(); //Returns a set of keys
    
    //And replace it in an existing uri
    uri->setQuery(newQuery->dump());
}
```
