# Contributing

## Project structure

1. Each module should have the following structure:
    ```
    module-name/
        CmakeLists.txt
        src/
            file.c
            ...
        include/
            file.h
            ...
    ``` 
2. Inside `include` folder there should be a file with the name `module-name.h` which contains only includes and
   re-exports all functionally.

## Coding guidelines

1. Use `#pragma once` in header files
2. Stick with linux coding style. For more information read here: [https://github.com/torvalds/linux/blob/master/Documentation/process/coding-style.rst](https://github.com/torvalds/linux/blob/master/Documentation/process/coding-style.rst)
3. Log errors and warnings.
   Use [centralized exiting of functions](https://github.com/torvalds/linux/blob/master/Documentation/process/coding-style.rst#7-centralized-exiting-of-functions):
```c
bool func() {
    bool success = true;
    struct bar foo = malloc(sizeof(bar));
    res = do_something();
    if (!res) {
        mlog(LOG_WARNING, "%s: failed to do something", __FUNCTION__);
        success = false;
        goto err_free_foo;
    }
    // other computations
err_free_foo:
    free(foo);
    return success;
}
```

## Commit guidelines
1. We follow Conventional Commits specification. For more information read here: [https://www.conventionalcommits.org/en/v1.0.0/](https://www.conventionalcommits.org/en/v1.0.0/)
