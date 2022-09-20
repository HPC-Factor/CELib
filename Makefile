# Makefile for celib
#
# Time-stamp: <03/02/01 14:38:19 keuchel@lightning>

#CFG = WINCE_DEBUGSTATIC
CFG = WINCE_RELEASESTATIC
#CFG = WINCE_MTRELEASESTATIC

TARGETCPU=SH3
#TARGETCPU=MIPS

# SDK Settings
OSVERSION  = WCE211
CEVERSION  = 211
CESUBSYS   = windowsce,2.11

PLATFORM   = MS HPC Pro
WCEROOT    = D:\Windows CE Tools
SDKROOT    = D:\Windows CE Tools

# Where to install lib and headers
LIBINSTDIR = H:/src/wince/lib
INCINSTDIR = H:/src/wince/include

RCDEFINES=-DUNDER_CE -DUNICODE -D_UNICODE -DWIN32 -DSTRICT \
	-Dx86 -Di486 -D_X86_ -D_WIN32_WCE=200 \
	-D_WIN32_WCE_EMULATION -DUSA \
	-DINTLMSG_CODEPAGE=1252

PATH=$(WCEROOT)\bin;$(WCEROOT)\$(OSVERSION)\bin;$(PATH)
INCLUDE=$(WCEROOT)/$(OSVERSION)/$(PLATFORM)/include
LIB=$(WCEROOT)/$(OSVERSION)/$(PLATFORM)/lib/$(TARGETCPU)

!message *****************************************************************
!message Building for $(TARGETCPU) $(PLATFORM) $(OSVERSION)
!message INCLUDE = $(INCLUDE)
!message LIB     = $(LIB)
!message *****************************************************************
!message

# I use unix tools for these...
MKDIR	= xmkdir
CP      = cp
RM      = rm -f 

######################################################################

# When subsystem is windowsce,2.0, the stack setting is generally
# ignored by wince 2.11 and set to 0x10000.
# When subsystem is windowsce,2.11, the stack flag has effects!
STACK          = -stack:0x020000
STACK          = -stack:0x050000

COMMONCFLAGS   = -nologo -I inc
COMMONCEDEFS   = -D_WINCE -DUNDER_CE=$(CEVERSION) -DBUILD_CELIB=1 #-DXCEDEBUG
LIBFLAGS       = -nologo
LDFLAGS        = -nologo -incremental:no -debug:full -pdb:none -nodefaultlib \
                 -subsystem:$(CESUBSYS) $(STACK)
LIBBASE        = celib
CELIBS         = corelibc.lib coredll.lib

######################################################################

!if "$(TARGETCPU)" == "X86"
COMMONCEDEFS = $(COMMONCEDEFS) -D _X86_
CPU = x86
!endif

!if "$(TARGETCPU)" == "MIPS"
CC = clmips.exe
COMMONCEDEFS = $(COMMONCEDEFS) -D MIPS
CPU = mips
!endif

!if "$(TARGETCPU)" == "SH3"
CC = shcl.exe
COMMONCEDEFS = $(COMMONCEDEFS) -D SHx
CPU = sh3
!endif

######################################################################

!if "$(CFG)" == "WINCE_RELEASESTATIC"
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -O2 
LIBNAME = $(LIBBASE)ce_$(CPU).lib
!endif

!if "$(CFG)" == "WINCE_DEBUGSTATIC"
PDBNAME = $(LIBBASE)ced_$(CPU).pdb
LIBNAME = $(LIBBASE)ced_$(CPU).lib
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -Od -Yd -Zi -D_DEBUG -Fd"$(PDBNAME)"
!endif

!if "$(CFG)" == "WINCE_MTRELEASESTATIC"
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -O2 -D_MT
LIBNAME = $(LIBBASE)cemt_$(CPU).lib
!endif

!if "$(CFG)" == "WINCE_MTDEBUGSTATIC"
PDBNAME = $(LIBBASE)cemtd_$(CPU).pdb
LIBNAME = $(LIBBASE)cemtd_$(CPU).lib
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -D_MT -Od -Yd -Zi -D_DEBUG \
                                         -Fd"$(PDBNAME)"
!endif

!if "$(CFG)" == "RELEASESTATIC"
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -O2
LIBNAME = $(LIBBASE).lib
!endif

!if "$(CFG)" == "DEBUGSTATIC"
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -Od -Yd -Zi -D_DEBUG
LIBNAME = $(LIBBASE)d.lib
!endif

!if "$(CFG)" == "RELEASESHARED"
CFLAGS = $(COMMONCFLAGS) -O2 -MD
LIBNAME = $(LIBBASE)rt.lib
!endif

!if "$(CFG)" == "DEBUGSHARED"
CFLAGS = $(COMMONCFLAGS) -Od -Yd -Zi -D_DEBUG -MDd
LIBNAME = $(LIBBASE)rtd.lib
!endif

!if "$(CFG)" == "MTRELEASESTATIC"
CFLAGS = $(COMMONCFLAGS) -O2 -MT
LIBNAME = $(LIBBASE)mt.lib
!endif

!if "$(CFG)" == "MTDEBUGSTATIC"
CFLAGS = $(COMMONCFLAGS) -Od -Yd -Zi -D_DEBUG -MTd
LIBNAME = $(LIBBASE)mtd.lib
!endif

######################################################################

OBJS = ceerrno.obj ceio.obj cestat.obj cewin32.obj ceutil.obj cestdlib.obj \
       cestr.obj cestdio.obj cesignal.obj cedebug.obj \
       cetoolhelp.obj \
       ceargv.obj cedir.obj ceenv.obj \
       snprintf.obj ftruncate.obj \
       wsstart.obj wsdb.obj wserr.obj wsstrerror.obj \
       rcmd.obj rexec.obj \
       ceprocess.obj cecursor.obj \
       fixpath.obj createfile.obj createfilemap.obj currentdir.obj \
       fileattr.obj deletefile.obj registry.obj findfile.obj \
       createdir.obj removedir.obj copyfile.obj movefile.obj \
       readwrite.obj \
       strncasecmp.obj alarm.obj bsd.obj sleep.obj syslog.obj \
       uid.obj gid.obj chown.obj \
       pwd.obj logwtmp.obj localhost.obj calloc.obj \
       makeunixpath.obj \
       localtim.obj asctime.obj time.obj ctime.obj gmtime.obj \
       ftime.obj dtoxtime.obj mktime.obj tzset.obj utime.obj \
       strpbrk.obj inet_aton.obj ascii2addr.obj \
       fnmatch.obj glob.obj getopt.obj cethread.obj \
       cert0.obj

XOBJS = cethread.obj

######################################################################

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	lib $(LIBFLAGS) -out:$(LIBNAME) $(OBJS)

install: all
	-$(MKDIR) $(INCINSTDIR)/sys 2>nul
	$(CP) cert0.obj  $(LIBINSTDIR)/cert0_$(CPU).obj
	$(CP) $(LIBNAME) $(LIBINSTDIR)
	$(CP) inc/*.h $(INCINSTDIR)
	$(CP) inc/sys/*.h $(INCINSTDIR)/sys

clean:
	$(RM) *.obj *.exe *.ilk $(LIBNAME) $(PDBNAME)

tests: test1.exe test1w.exe

# NOTE: toolhelp.dll must be copied to device for test1...
test1.exe: test1.obj $(LIBNAME)
	link $(LDFLAGS) -out:test1.exe \
             h:\src\wince\lib\cert0_$(CPU).obj test1.obj \
             $(LIBNAME) $(CELIBS) toolhelp.lib winsock.lib

test1w.res: test1w.rc
	rc $(RCDEFINES) test1w.rc

test1w.exe: test1w.obj test1w.res $(LIBNAME)
	link $(LDFLAGS) -out:test1w.exe test1w.obj test1w.res \
             $(LIBNAME) $(CELIBS) commctrl.lib winsock.lib

!if "$(TARGETCPU)" == "SH3"
install_tests: tests
	cecopy pc:test1.exe ce:/speicherkarte2/bin/test1.exe
#	cecopy pc:test1w.exe ce:/speicherkarte2/bin/test1w.exe
!endif

!if "$(TARGETCPU)" == "MIPS"
install_tests: tests
	cecopy pc:test1.exe ce:/netzwerk/compaq/bin/test1.exe
#	cecopy pc:test1w.exe ce:/netzwerk/compaq/bin/test1w.exe
!endif

run_test:
	ceexec /speicherkarte2/bin/test1.exe

#$(OBJS): inc/celib.h inc/cethread.h
$(OBJS): inc/cethread.h

