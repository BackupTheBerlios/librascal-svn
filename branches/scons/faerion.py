"""faerion

Faerion macros for SCons.

"""

#
# Copyright (C) 2005 The Faerion Team
#
# $Id$
#

import os
import re
import shutil
import string
import SCons.Defaults

class Faerion:
	def listFilesW(self, at, r):
		nodes = []
		for location in at:
			for file in os.listdir(location):
				fname = os.path.join(location, file)
				if os.path.isfile(fname):
					if r.match(file):
						nodes.append(fname)
				elif os.path.isdir(fname):
					if not os.path.basename(fname) == '.svn':
						for more in self.listFilesW([fname], r):
							nodes.append(more)
		return nodes

	def listFiles(self, at, r):
		l = self.listFilesW(at, re.compile(r))
		l.sort()
		return l

	def compile(self, env, paths):
		objs = []
		for item in self.listFiles(paths, r'^.*(\.cc)$'):
			fname, ext = os.path.splitext(item)
			if env.has_key('category'):
				fname = fname + '~' + env['category']
			if env['debug'] == 1:
				fname = fname + '~debug'
			objs.append(env.SharedObject(fname + '.o', item))
		return objs

	def makeInstaller(self, xenv, root, name, fmask):
		dir = name
		tar = name + '-src.tar.gz'

		if xenv.GetOption('clean'):
			if os.path.exists(tar):
				os.unlink(tar)
		else:
			env = xenv.Copy(TARFLAGS = '-c -z')
			# the list of created directories, to avoid
			# excessive filesystem calls.
			edirs = []

			if os.path.exists(dir):
				shutil.rmtree(dir)

			for item in self.listFiles([root], fmask):
				f = env.File(item)

				src = f.path
				dst = dir + '/' + os.path.dirname(f.path)
				dsf = dir + '/' + f.path

				if not dst in edirs:
					if not os.path.exists(dst):
						os.makedirs(dst)
						edirs = edirs + [dst]

				shutil.copy2(src, dsf)
				env.Tar(tar, dsf)

			env.AddPostAction(tar, SCons.Defaults.Delete(dir))

	# Builds a configuration object.
	def Configure(self, conf, env):
		print 'Configuring for platform: ' + env['PLATFORM'] + '.'

		if os.environ.has_key('CPATH'):
			for em in string.split(os.environ['CPATH'], ';'):
				env.Append(CPPPATH = em)
				print 'Found directory %s in the environment.' % (em)

		return conf

	# Post-configuration.
	def FinishConf(self, conf):
		env = conf.Finish()

		# Adapt to a debug build.
		if env['debug'] == 1:
			print '*** DEBUG BUILD ***'
			env.Append(CPPDEFINES = '_DEBUG')
			if env['CC'] == 'cl':
				env.Append(CCFLAGS = '/Od /GX /EHsc /MDd /Z7')
				env.Append(LINKFLAGS = '/DEBUG /OPT:NOICF /OPT:NOREF')
			elif env['CC'] == 'gcc':
				env.Append(CCFLAGS = '-g -O0')

		# Adapt to a release build.
		else:
			if env['CC'] == 'cl':
				env.Append(CCFLAGS = '/Ox /G5 /GF /Gy /MD')
				env.Append(LINKFLAGS = '/RELEASE /OPT:REF /OPT:ICF')
			elif env['CC'] == 'gcc':
				env.Append(CCFLAGS = '-O3 -fomit-frame-pointer -finline-functions -fno-exceptions')
				env.Append(LINKFLAGS = '-s --gc-sections')

		return env

# vim:ts=4:sts=4:sw=4:noet:nocindent:syntax=python
