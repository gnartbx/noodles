cc_binary(
    name = "nq",
    srcs = ["nq.cc"],
    deps = [":queens"],
)

cc_library(
    name = "queens",
    hdrs = ["queens.h"],
    srcs = ["queens.cc"],
)

cc_test(
    name = "queens_test",
    size = "small",
    srcs = ["queens_test.cc"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        ":queens",
    ],
)
