
import os
import sys
import time
import psutil

msvc = 0
clang = 1

full_debug = 0
optimized_debug = 1
optimized = 2

compiler = msvc
debug = full_debug

def main():
	global compiler

	os.system("cls")
	os.system("del build\\*.pdb > NUL 2> NUL")

	if len(sys.argv) != len(set(sys.argv)):
		print("Error: Duplicate arguments")
		return

	in_sanitize = False
	do_time = False

	for arg in sys.argv[1:]:
		if arg == "sanitize":
			in_sanitize = True
		elif arg == "clang":
			compiler = clang
		elif arg == "time":
			do_time = True
		else:
			print(f"Error: Unknown argument '{arg}'")
			return

	if do_time:
		start_time = time.time()

	success = compile(
		["..\\src\\main.cpp"], name="lang",
		libs=["user32", "OpenGL32", "Shell32", "Ole32", "Shlwapi", "Winmm"], no_console_in_release=True,
		sanitize=in_sanitize
	)

	if do_time:
		print(f"Done in {time.time() - start_time:.3f} seconds")

	if not success:
		play_fail_sound()
		return

	play_success_sound()
	os.system("copy build\lang.exe lang.exe > NUL")

def play_success_sound():
	os.system("start /MIN \"\" cmdmp3 build_success.mp3")

def play_fail_sound():
	os.system("start /MIN \"\" cmdmp3 build_fail.mp3")

def get_defines(debug):
	defines = []
	if debug == full_debug or debug == optimized_debug:
		defines.append("m_debug")
	return defines

def compile(files, name="main", do_dll=False, pdb_name=None, include_dirs=[], libs=[], no_console_in_release=False, sanitize=False):
	assert len(files) > 0

	defines = get_defines(debug)

	comp_msvc = [
		"-std:c++20",
		"-nologo",
		"-Zc:strictStrings-",
		"-D_CRT_SECURE_NO_WARNINGS",
		"/EHa-",
		"/EHs-",
		"/GR-",
		"/GS-",
		"/Oi",
		"-FC",
		"-Gm-",
		"-Wall",
		"-wd 4820",
		"-wd 4820",
		"-wd 4365",
		"-wd 4201",
		"-wd 5219",
		"-wd 5246",
		"-wd 5220",
		"-wd 4464",
		"-wd 4623",
		"-wd 5045",
		"-wd 4710",
		"-wd 4514", # Unreferenced function has been removed
		"-wd 4505", # Unreferenced function with internal linkage has been removed
		"-wd 4061", # Enum member not handled on switch
		"-wd 4062", # Same thing
		"-wd 4686",
		"-wd 4626", # Assignment operator was implicity defined as deleted
		"-wd 4191",
		"-wd 4711",
		"-wd 4324", # Structure was padded due to alignment modifier
	]

	comp_clang = [
		"-std=c++20",
		"-Wextra",
		"-Wpedantic",
		"-Wall",
		"-Weverything",
		"-Wno-switch",
		"-Wno-c99-designator",
		"-Wno-writable-strings",
		"-Wno-gnu-anonymous-struct",
		"-Wno-nested-anon-types",
		"-Wno-language-extension-token",
		"-Wno-sign-compare",
		"-Wno-missing-braces",
		"-Wno-logical-op-parentheses",
		"-Wno-char-subscripts",
		"-Wno-c++98-compat-pedantic",
		"-Wno-non-virtual-dtor",
		"-Wno-suggest-override",
		"-Wno-extra-semi-stmt",
		"-Wno-old-style-cast",
		"-Wno-double-promotion",
		"-Wno-reserved-macro-identifier",
		"-Wno-sign-conversion",
		"-Wno-implicit-int-float-conversion",
		"-Wno-missing-noreturn",
		"-Wno-string-conversion",
		"-Wno-covered-switch-default",
		"-Wno-switch-enum",
		"-Wno-c++2b-extensions",
		"-Wno-microsoft-cast",
		"-Wno-unused-function",
		"-Wno-enum-constexpr-conversion",

		"-Wno-zero-as-null-pointer-constant",
		"-Wno-unused-template",
		"-Wno-unsafe-buffer-usage",
		"-Wno-newline-eof",
		"-Wno-cast-align",
		"-Wno-tautological-type-limit-compare",
		"-Wno-missing-prototypes",
		"-Wno-cast-function-type-strict",
		"-Wno-cast-function-type",
		"-Wno-global-constructors",
		"-Wno-shadow-field-in-constructor",
		"-Wno-format-nonliteral",
		"-Wno-missing-variable-declarations",
		"-Wno-deprecated-declarations",

		"-Wno-microsoft-goto",
	]

	if sanitize:
		comp_msvc.append("/fsanitize=address")
		comp_clang.append("/fsanitize=address")

	link_msvc = [
		"/STACK:0x2000000,0x2000000",
		"-INCREMENTAL:NO",
	]
	link_clang = []

	for i in include_dirs:
		comp_msvc.append(f"-I{i}")
		comp_clang.append(f"-I{i}")

	if pdb_name != None:
		link_msvc.append(f"/PDB:{pdb_name}.pdb")

	if debug == full_debug:
		comp_msvc.append("/Od")
		comp_msvc.append("/MTd")
		comp_msvc.append("/Zi")
		comp_clang.append("-g")

	if debug == optimized_debug:
		comp_msvc.append("/O2")
		comp_msvc.append("/MTd")
		comp_msvc.append("/Zi")
		comp_clang.append("-g")

	if debug == optimized:
		comp_msvc.append("/O2")
		comp_msvc.append("/MT")

	if no_console_in_release and debug == optimized:
		link_msvc.append("-SUBSYSTEM:WINDOWS")

	if not "m_debug" in comp_msvc:
		comp_msvc.append("-wd 4127")

	if do_dll:
		comp_msvc.append("-LD")
		comp_msvc.append(f"-Fe:{name}.dll")
		comp_clang.append("-shared")
		comp_clang.append(f"-o {name}.dll")

	else:
		comp_msvc.append(f"-Fe:{name}.exe")
		comp_clang.append(f"-o {name}.exe")

	for d in defines:
		comp_msvc.append(f"-D{d}")
		comp_clang.append(f"-D{d}")

	cmd = ""
	cmd += "pushd build && "

	if compiler == msvc:
		cmd += f"cl {' '.join(files)}"
		for c in comp_msvc:
			cmd += f" {c}"
	elif compiler == clang:
		cmd += f"clang {' '.join(files)}"
		for c in comp_clang:
			cmd += f" {c}"

	if compiler == msvc:
		cmd += " -link"
		for l in libs:
			cmd += f" {l}.lib"
		for l in link_msvc:
			cmd += f" {l}"
	elif compiler == clang:
		for l in libs:
			cmd += f" -l{l}"
		for l in link_clang:
			cmd += f" {l}"

	cmd += " > temp_compiler_output.txt && popd"
	success = True if os.system(cmd) == 0 else False
	os.system("popd")
	# print(cmd)
	if success and compiler == clang and pdb_name != None:
		os.system(f"cd build && copy {name}.pdb {pdb_name}.pdb > NUL")

	os.system("type build\\temp_compiler_output.txt")
	os.system("copy build\\temp_compiler_output.txt compiler_output.txt > NUL")

	return success

if __name__ == "__main__":
	main()