-- lsgpu.lua
-- Small CLI wrapper for the lsgpu Lua module, mirroring lsgpu_cli.c

package.cpath = package.cpath .. ";../build/lib/lua/?.so"

local lsgpu = require("lsgpu_lua")

local function usage()
	io.stderr:write("Usage: lsgpu.lua [-b <filename>] | [-d <filename>]\n")
	os.exit(1)
end

local function main()
	local argc = #arg
	local binary_filename
	local do_encode = false
	local do_decode = false

	if not (argc == 0 or argc == 2) then
		usage()
	end

	if argc == 2 then
		if arg[1] == "-b" then
			do_encode = true
		elseif arg[1] == "-d" then
			do_decode = true
		else
			usage()
		end
		binary_filename = arg[2]
	end

	local ok = lsgpu.init()
	if not ok then
		io.stderr:write("Failed to init lsgpu\n")
		return 1
	end

	local gpus, errmsg

	if do_encode then
		gpus, errmsg = lsgpu.query_gpus()
		if not gpus then
			io.stderr:write("Failed to query GPU devices: ", tostring(errmsg), "\n")
			lsgpu.fini()
			return 1
		end

		local wrote, werr = lsgpu.write_binary(gpus, binary_filename)
		if not wrote then
			io.stderr:write("Failed to write GPU data to binary file '", binary_filename, "': ", tostring(werr), "\n")
			lsgpu.fini()
			return 1
		end

		print(string.format("GPU data written to binary file '%s' successfully.", binary_filename))

	elseif do_decode then
		gpus, errmsg = lsgpu.read_binary(binary_filename)
		if not gpus then
			io.stderr:write("Failed to read GPU data from binary file '", binary_filename, "': ", tostring(errmsg), "\n")
			lsgpu.fini()
			return 1
		end

		local printed, perr = lsgpu.print(gpus)
		if not printed then
			io.stderr:write("Failed to print GPU data: ", tostring(perr), "\n")
			lsgpu.fini()
			return 1
		end

	else
		gpus, errmsg = lsgpu.query_gpus()
		if not gpus then
			io.stderr:write("Failed to query GPU devices: ", tostring(errmsg), "\n")
			lsgpu.fini()
			return 1
		end

		local printed, perr = lsgpu.print(gpus)
		if not printed then
			io.stderr:write("Failed to print GPU data: ", tostring(perr), "\n")
			lsgpu.fini()
			return 1
		end
	end

	local fini_ok = lsgpu.fini()
	if not fini_ok then
		io.stderr:write("Failed to fini lsgpu\n")
		return 1
	end

	return 0
end

os.exit(main())

