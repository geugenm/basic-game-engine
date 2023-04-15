include(../CPM.cmake.cmake)

CPMAddPackage(
        NAME ccache
        GITHUB_REPOSITORY "TheLartians/Ccache.cmake"
        VERSION 1.2.4
        GIT_TAG "v1.2.4"
)

set(DUSE_CCACHE YES)
set(DCCACHE_OPTIONS "CCACHE_CPP2=true;CCACHE_SLOPPINESS=clang_index_store")
