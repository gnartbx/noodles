cc_binary(
    name = "atax",
    srcs = ["atax.cc"],
    deps = [
        "//external:gflags",
	":board"
    ],
)

cc_library(
    name = "board",
    hdrs = ["board.h"],
    srcs = ["board.cc"],
)

cc_test(
    name = "board_test",
    size = "small",
    srcs = ["board_test.cc"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        ":board",
    ],
)
