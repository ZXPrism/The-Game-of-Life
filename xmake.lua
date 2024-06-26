set_project("TGOL")

add_rules("mode.debug", "mode.release")
add_requires("libsdl")

target("life")
    set_languages("c99", "cxx20")
    set_kind("binary")
    set_warnings("all")

    add_files("src/**.cpp")
    add_includedirs("src")
    add_packages("libsdl")

    after_build(function (target)
        os.cp(target:targetfile(), "bin/")
    end)
target_end()
