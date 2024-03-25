#!/usr/bin/python3.8
import sys
import datetime
import string
import random
import hashlib

base_functions = {\
	"open": 	["int32_t", 	"open_m", 	"(uint8_t min)"],\
	"close": 	["int32_t", 	"close_m", 	"(uint8_t min)"], \
	"read": 	["int32_t", 	"read_m", 	"(uint8_t min, void *buf, uint16_t size)"],
	"write": 	["int32_t", 	"write_m", 	"(uint8_t min, void *buf, uint16_t size)"],
	"unlocked_ioctrl": ["int32_t", "_ioctrl_m", "(uint8_t min, void *arg, uint16_t cmd)"],
    "seek":     ["void",        "seek_m",   "(uint8_t min, uint32_t offset, uint8_t whence)"]
};

ERROR_MSG_0 = "Name the device driver. Tips:\n\
	- Keep it simple\n\
	- Keep it short\n\
	- Use underscore to separete words\n\
	- keep it lower case, cause it will be upppercased"

# VERSIONING ------------------------------------------------
version = ""
version_date = ""
version_table = "versiontable.dat"

def check_version():
	global version
	global version_date

	currentHash = ""
	# A version is represented as follows "versionA.VersionB.VersionC", each field
	# increases up to ten and the the following up version goes up 1 unit. i.e.:
	# 0.0.0 -> 0.0.1 -> ... -> 0.0.9 -> 0.1.0
	version_digits = [0] * 3
	# Compute the Hash md5 of this file (mdev_gen.py)
	with open(sys.argv[0], "rb") as f:
		currentHash = hashlib.md5(f.read()).hexdigest()

	path_version_table = "./" + '/'.join(sys.argv[0].split("/")[:-1]) + '/' + version_table

	# found the hash inside the version table and appended if not found
	version_found = False
	last_line = ""
	with open(path_version_table, 'r') as f:
		for line in f:
			last_line = line
			fields = line.split('\t')
			if (fields[0] == currentHash):
				version_found = True
				version = fields[1]
				version_date = fields[2]
				break

	if not version_found:
		sys.stderr.write("New version of mdev generator found, registering...\n")
		fields = last_line.split('\t')
		version = fields[1].split('.')
		version_digits[0] = int(version[0])
		version_digits[1] = int(version[1])
		version_digits[2] = int(version[2])

		version_digits[2] += 1
		if (version_digits[2] > 99):
			version_digits[2] = 0;
			version_digits[1] += 1

		if (version_digits[1] > 9):
			version_digits[2] = 0
			version_digits[1] = 0
			version_digits[0] += 1

		version = '.'.join([str(i) for i in version_digits])
		version_date = str(datetime.datetime.now())
		comment = input("Comment this version:\n")

		with open(path_version_table,'a') as f:
			f.write(currentHash+"\t"+version+"\t"+version_date+"\t"+comment+"\n")
#-----------------------------------------------------------

# GENERATOR ------------------------------------------------
def savefile(filename, content):
	f = open(filename,'w');
	f.write(content)
	f.close()

def generate_commment_multiline(comment):
	new_comment = ""
	i = 0
	while i < 70:
		offset = 70;
		new_comment = new_comment + " * " + comment[i:i + offset]
		new_comment = new_comment + "\n"
		i = i + offset;
	return new_comment + " * " + comment[i:]

def generate_initial_comment(mod_name):
	file_h = "/*********************************************************************\n"
	file_h += generate_commment_multiline("The skeleton of this file was\
 generated automatically for the mdev    framework that supports the module:\n *\t\t" + mod_name)
	file_h += "\n * \n"
	file_h += " * File generated on " + str(datetime.datetime.now()) + "\n"
	file_h += " * With mdev generator version " + version + " from " + version_date + "\n"
	file_h += " ********************************************************************/\n\n"
	return file_h

def generate_function(mod_name, foo, args, returnType, comment):
	file_c = "\n/*\n"
	file_c += " * ## " + foo + mod_name  + args + "\n"
	file_c += " *\n"
	file_c += comment + "\n"
	file_c += " */\n"
	file_c += returnType + " " + foo + mod_name + args +" {\n"
	return file_c

# C File GENERATOR -----------------------------------------------------------------
def generate_c_file(mod_name):
	mod_nameUP = mod_name.upper()
	file_c = generate_initial_comment(mod_name)

	file_c += "#ifdef _MDEV_USE_" + mod_nameUP + "_\n\n\n" 

	file_c += "#include \"mdev.h\"\n"
	file_c += "#include \"" + mod_name + ".h\"\n\n"

	# device data struct
	file_c += "/*\nstruct m" + mod_name + " {" + "\n"
	file_c += "\t// m" + mod_name + "'s data \n"
	file_c += "};\n\n"

	file_c += "static struct m" + mod_name + " " + mod_name +\
	"s [" + mod_nameUP + "_MINORS];\n"
	file_c += "*/\n\n"

	# Definition of operator functions
	for f in base_functions:
		file_c = file_c + generate_function(\
			mod_name,\
			base_functions[f][1], \
			base_functions[f][2], \
			"/*\n" + base_functions[f][0],\
			generate_commment_multiline("[Edit]\
 Uncomment the function to use. This is the comment section, you can write comments\
 here for documentation purposes.")\
		)
		file_c = file_c + "\treturn 1;\n}\n*/\n"

	# Definition of init function
	file_c = file_c + generate_function(mod_name, "ModuleInit_m", "()", "int8_t", "")
	file_c = file_c + "\t// uint8_t i;\n\n"
	file_c = file_c + "\tif(mdev_init(" + mod_nameUP + "_MINORS, devs_" + mod_name + ") < 0)\n"
	file_c = file_c + "\t\treturn -1;\n"
	file_c = file_c + "\t/*\n\tfor (i = 0; i < " + mod_nameUP + "_MINORS; i++) {\n"
	file_c = file_c + "\t\t// Initalize mods[i]' fields\n"
	file_c = file_c + "\t}\n\t*/\n"
	file_c = file_c + "\treturn 0;\n"
	file_c = file_c + "}\n"

	file_c = file_c + "\n\n\n#endif /* _MDEV_USE_" + mod_nameUP + "_*/\n"


	savefile(mod_name + ".c", file_c)

# HEADER FILE GENERATOR -----------------------------------------------------------
def generate_header(mod_name):
	mod_nameUP = mod_name.upper()
	header_definition = mod_nameUP + "_H"

	file_c = generate_initial_comment(mod_name)
	file_c += "#ifdef _MDEV_USE_" + mod_nameUP + "_\n\n\n" 

	file_c += "#ifndef " + header_definition + "\n"
	file_c += "#define " + header_definition + "\n\n"
	file_c += "#define " + mod_nameUP + "_MINORS 1\t// [Edit] to include more minors\n\n"
	file_c += "// #define ioctrl_m" + mod_name + "(mi, cmd, arg) _ioctrl_m" + mod_name + "(mi, arg, cmd)\n\n"
	file_c += "int8_t ModuleInit_m" + mod_name + "();\n\n\n"

	file_c += "/*\n"
	file_c += generate_commment_multiline("List of operators by default, some devices might need them all.")
	file_c += "*/\n\n"
	for f in base_functions:
		file_c += "// " + base_functions[f][0] + " " + base_functions[f][1] + mod_name + base_functions[f][2] + ";\n"

	file_c = file_c + "\n#endif /*" + header_definition + "*/\n"
	
	file_c = file_c + "\n\n\n#endif /* _MDEV_USE_" + mod_nameUP + "_*/\n"
	
	savefile(mod_name + ".h", file_c);

#----------------------------------------------------------------------------------


def id_generatior(size=3, chars=string.ascii_lowercase + string.digits):
	return ''.join(random.choice(chars) for _ in range(size))

def main():
	if (len(sys.argv) < 2):
		sys.stderr.write(ERROR_MSG_0)
		return
	check_version()
	# mod_name = sys.argv[1] + id_generatior()
	mod_name = sys.argv[1]
	generate_header(mod_name)
	generate_c_file(mod_name)


if __name__ == '__main__':
	main()
