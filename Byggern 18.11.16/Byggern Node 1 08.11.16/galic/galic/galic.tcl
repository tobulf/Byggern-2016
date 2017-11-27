
########## Tcl recorder starts at 09/06/16 17:50:03 ##########

set version "2.0"
set proj_dir "C:/Users/henniw/Desktop/galic"
cd $proj_dir

# Get directory paths
set pver $version
regsub -all {\.} $pver {_} pver
set lscfile "lsc_"
append lscfile $pver ".ini"
set lsvini_dir [lindex [array get env LSC_INI_PATH] 1]
set lsvini_path [file join $lsvini_dir $lscfile]
if {[catch {set fid [open $lsvini_path]} msg]} {
	 puts "File Open Error: $lsvini_path"
	 return false
} else {set data [read $fid]; close $fid }
foreach line [split $data '\n'] { 
	set lline [string tolower $line]
	set lline [string trim $lline]
	if {[string compare $lline "\[paths\]"] == 0} { set path 1; continue}
	if {$path && [regexp {^\[} $lline]} {set path 0; break}
	if {$path && [regexp {^bin} $lline]} {set cpld_bin $line; continue}
	if {$path && [regexp {^fpgapath} $lline]} {set fpga_dir $line; continue}
	if {$path && [regexp {^fpgabinpath} $lline]} {set fpga_bin $line}}

set cpld_bin [string range $cpld_bin [expr [string first "=" $cpld_bin]+1] end]
regsub -all "\"" $cpld_bin "" cpld_bin
set cpld_bin [file join $cpld_bin]
set install_dir [string range $cpld_bin 0 [expr [string first "ispcpld" $cpld_bin]-2]]
regsub -all "\"" $install_dir "" install_dir
set install_dir [file join $install_dir]
set fpga_dir [string range $fpga_dir [expr [string first "=" $fpga_dir]+1] end]
regsub -all "\"" $fpga_dir "" fpga_dir
set fpga_dir [file join $fpga_dir]
set fpga_bin [string range $fpga_bin [expr [string first "=" $fpga_bin]+1] end]
regsub -all "\"" $fpga_bin "" fpga_bin
set fpga_bin [file join $fpga_bin]

if {[string match "*$fpga_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$fpga_bin;$env(PATH)" }

if {[string match "*$cpld_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$cpld_bin;$env(PATH)" }

lappend auto_path [file join $install_dir "ispcpld" "tcltk" "lib" "ispwidget" "runproc"]
package require runcmd

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 17:50:03 ###########


########## Tcl recorder starts at 09/06/16 17:54:13 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 17:54:13 ###########


########## Tcl recorder starts at 09/06/16 17:55:02 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 17:55:02 ###########


########## Tcl recorder starts at 09/06/16 17:55:39 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 17:55:39 ###########


########## Tcl recorder starts at 09/06/16 17:56:32 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 17:56:32 ###########


########## Tcl recorder starts at 09/06/16 18:09:03 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:09:03 ###########


########## Tcl recorder starts at 09/06/16 18:09:20 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:09:20 ###########


########## Tcl recorder starts at 09/06/16 18:10:07 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:10:07 ###########


########## Tcl recorder starts at 09/06/16 18:10:12 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:10:12 ###########


########## Tcl recorder starts at 09/06/16 18:29:35 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:29:35 ###########


########## Tcl recorder starts at 09/06/16 18:30:09 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:30:09 ###########


########## Tcl recorder starts at 09/06/16 18:31:26 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:31:26 ###########


########## Tcl recorder starts at 09/06/16 18:34:09 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:34:09 ###########


########## Tcl recorder starts at 09/06/16 18:34:16 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:34:16 ###########


########## Tcl recorder starts at 09/06/16 18:34:52 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:34:52 ###########


########## Tcl recorder starts at 09/06/16 18:34:58 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:34:58 ###########


########## Tcl recorder starts at 09/06/16 18:37:09 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:37:09 ###########


########## Tcl recorder starts at 09/06/16 18:37:14 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:37:14 ###########


########## Tcl recorder starts at 09/06/16 18:39:31 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/16 18:39:31 ###########


########## Tcl recorder starts at 09/13/16 11:47:42 ##########

set version "2.0"
set proj_dir "C:/Users/tobiasu/Documents/Byggern/galic/galic"
cd $proj_dir

# Get directory paths
set pver $version
regsub -all {\.} $pver {_} pver
set lscfile "lsc_"
append lscfile $pver ".ini"
set lsvini_dir [lindex [array get env LSC_INI_PATH] 1]
set lsvini_path [file join $lsvini_dir $lscfile]
if {[catch {set fid [open $lsvini_path]} msg]} {
	 puts "File Open Error: $lsvini_path"
	 return false
} else {set data [read $fid]; close $fid }
foreach line [split $data '\n'] { 
	set lline [string tolower $line]
	set lline [string trim $lline]
	if {[string compare $lline "\[paths\]"] == 0} { set path 1; continue}
	if {$path && [regexp {^\[} $lline]} {set path 0; break}
	if {$path && [regexp {^bin} $lline]} {set cpld_bin $line; continue}
	if {$path && [regexp {^fpgapath} $lline]} {set fpga_dir $line; continue}
	if {$path && [regexp {^fpgabinpath} $lline]} {set fpga_bin $line}}

set cpld_bin [string range $cpld_bin [expr [string first "=" $cpld_bin]+1] end]
regsub -all "\"" $cpld_bin "" cpld_bin
set cpld_bin [file join $cpld_bin]
set install_dir [string range $cpld_bin 0 [expr [string first "ispcpld" $cpld_bin]-2]]
regsub -all "\"" $install_dir "" install_dir
set install_dir [file join $install_dir]
set fpga_dir [string range $fpga_dir [expr [string first "=" $fpga_dir]+1] end]
regsub -all "\"" $fpga_dir "" fpga_dir
set fpga_dir [file join $fpga_dir]
set fpga_bin [string range $fpga_bin [expr [string first "=" $fpga_bin]+1] end]
regsub -all "\"" $fpga_bin "" fpga_bin
set fpga_bin [file join $fpga_bin]

if {[string match "*$fpga_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$fpga_bin;$env(PATH)" }

if {[string match "*$cpld_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$cpld_bin;$env(PATH)" }

lappend auto_path [file join $install_dir "ispcpld" "tcltk" "lib" "ispwidget" "runproc"]
package require runcmd

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/13/16 11:47:42 ###########


########## Tcl recorder starts at 09/13/16 12:00:57 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/13/16 12:00:57 ###########


########## Tcl recorder starts at 09/13/16 12:01:13 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/13/16 12:01:13 ###########


########## Tcl recorder starts at 09/13/16 12:01:52 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/13/16 12:01:52 ###########


########## Tcl recorder starts at 09/13/16 12:01:56 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/13/16 12:01:56 ###########


########## Tcl recorder starts at 09/13/16 12:29:43 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/13/16 12:29:43 ###########


########## Tcl recorder starts at 09/20/16 15:03:04 ##########

set version "2.0"
set proj_dir "C:/Users/bragesae/Documents/Byggern/galic/galic"
cd $proj_dir

# Get directory paths
set pver $version
regsub -all {\.} $pver {_} pver
set lscfile "lsc_"
append lscfile $pver ".ini"
set lsvini_dir [lindex [array get env LSC_INI_PATH] 1]
set lsvini_path [file join $lsvini_dir $lscfile]
if {[catch {set fid [open $lsvini_path]} msg]} {
	 puts "File Open Error: $lsvini_path"
	 return false
} else {set data [read $fid]; close $fid }
foreach line [split $data '\n'] { 
	set lline [string tolower $line]
	set lline [string trim $lline]
	if {[string compare $lline "\[paths\]"] == 0} { set path 1; continue}
	if {$path && [regexp {^\[} $lline]} {set path 0; break}
	if {$path && [regexp {^bin} $lline]} {set cpld_bin $line; continue}
	if {$path && [regexp {^fpgapath} $lline]} {set fpga_dir $line; continue}
	if {$path && [regexp {^fpgabinpath} $lline]} {set fpga_bin $line}}

set cpld_bin [string range $cpld_bin [expr [string first "=" $cpld_bin]+1] end]
regsub -all "\"" $cpld_bin "" cpld_bin
set cpld_bin [file join $cpld_bin]
set install_dir [string range $cpld_bin 0 [expr [string first "ispcpld" $cpld_bin]-2]]
regsub -all "\"" $install_dir "" install_dir
set install_dir [file join $install_dir]
set fpga_dir [string range $fpga_dir [expr [string first "=" $fpga_dir]+1] end]
regsub -all "\"" $fpga_dir "" fpga_dir
set fpga_dir [file join $fpga_dir]
set fpga_bin [string range $fpga_bin [expr [string first "=" $fpga_bin]+1] end]
regsub -all "\"" $fpga_bin "" fpga_bin
set fpga_bin [file join $fpga_bin]

if {[string match "*$fpga_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$fpga_bin;$env(PATH)" }

if {[string match "*$cpld_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$cpld_bin;$env(PATH)" }

lappend auto_path [file join $install_dir "ispcpld" "tcltk" "lib" "ispwidget" "runproc"]
package require runcmd

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:03:04 ###########


########## Tcl recorder starts at 09/20/16 15:36:04 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:36:04 ###########


########## Tcl recorder starts at 09/20/16 15:36:19 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:36:19 ###########


########## Tcl recorder starts at 09/20/16 15:36:48 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:36:48 ###########


########## Tcl recorder starts at 09/20/16 15:39:45 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:39:45 ###########


########## Tcl recorder starts at 09/20/16 15:40:16 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:40:16 ###########


########## Tcl recorder starts at 09/20/16 15:41:47 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:41:47 ###########


########## Tcl recorder starts at 09/20/16 15:41:51 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:41:51 ###########


########## Tcl recorder starts at 09/20/16 15:42:22 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:42:22 ###########


########## Tcl recorder starts at 09/20/16 15:42:27 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 15:42:27 ###########


########## Tcl recorder starts at 09/20/16 17:50:18 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 17:50:18 ###########


########## Tcl recorder starts at 09/20/16 17:50:54 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 17:50:54 ###########


########## Tcl recorder starts at 09/20/16 18:50:20 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 18:50:20 ###########


########## Tcl recorder starts at 09/20/16 18:50:34 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"galic.vhd\" -o \"galic.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 18:50:34 ###########


########## Tcl recorder starts at 09/20/16 18:50:54 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open address_decoder.cmd w} rspFile] {
	puts stderr "Cannot create response file address_decoder.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: galic.sty
PROJECT: address_decoder
WORKING_PATH: \"$proj_dir\"
MODULE: address_decoder
VHDL_FILE_LIST: galic.vhd
OUTPUT_FILE_NAME: address_decoder
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e address_decoder -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete address_decoder.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"address_decoder.edi\" -out \"address_decoder.bl0\" -err automake.err -log \"address_decoder.log\" -prj galic -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"address_decoder.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"address_decoder.bl1\" -o \"galic.bl2\" -omod address_decoder -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" galic.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" galic.bl3 -pla -o galic.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" galic.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" galic.tt3 -dev p16v8 -o galic.jed -ivec NoInput.tmv -rep galic.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/16 18:50:54 ###########

