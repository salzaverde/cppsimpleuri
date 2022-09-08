# cppsimpleuri
A slim & simple to use uri parser.

Since this is my first open source project i'm happy about feedback and PR's in order to improve or extend this little library.

## Design goals
- clean
- maintainable
- extendable
- tested
- easy to integrate with cmake

The initial design is based on some requirements from another project. It is by no means completebut open to change.

## Requirements
- cmake
- c++20

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

int main(int argc, char *argv[]) {
    //Parsing
    auto myRawUri = std::string("https://myHomepage.com:1176/my/music?added=last");
    auto uri = salzaverde::URI::parse(myRawUri);
    
    //Access the uri's elements
    auto scheme = uri->getScheme();
    auto host = uri->getHost();
    auto port = uri->getPort();
    auto path = uri->getPath();
    auto query = uri->getQuery();
    auto queryString = query->dump();
    
    //Add or replace a query parameter
    query->set("sortby", "name");
    
    //Retrieve a query parameter value by key
    auto sortbyValue = query->get("sortby");
    
    //A value for this key might not exist
    if(! sortbyValue) {
        //Do some error handling
    }
       
    //We can also build a new query string in place
    auto newQuery = salzaverde::URI::Query::build({
        {"list", "songs"},
        {"include", "rhcp"}
    });

    //You can check if a key exists
    auto containsListKey = newQuery->contains("list");
    auto keys = query->listKeys(); //Returns a set of keys
    
    //And replace it in an existing uri
    uri->setQuery(std::move(newQuery));
}
```
