# vim: syntax=python

import os

arch = ARGUMENTS.get('arch', 'i386')
buildtype = ARGUMENTS.get('buildtype', 'debug')
ansi = ARGUMENTS.get('ansi', 'no')
strict = ARGUMENTS.get('strict', 'yes')

env = Environment(
	ENV = {'PATH' : os.environ['PATH']},
	OBJPREFIX='',
	OBJSUFFIX='.o',
	SHOBJPREFIX='',
	SHOBJSUFFIX='.sho',
	PROGPREFIX='',
	PROGSUFFIX='',
	LIBPREFIX='',
	LIBSUFFIX='.lib',
	SHLIBPREFIX='',
	SHLIBSUFFIX='.shl',
	CC='gcc',
	CCFLAGS=['-g', '-I', 'include'],
	AS='nasm',
)

if buildtype == 'debug':
	env.Append(CCFLAGS=['-g', '-D', 'DEBUG'], LINKFLAGS=['-g'])

Export('env', 'arch', 'buildtype')

SConscript('src/SConscript')
