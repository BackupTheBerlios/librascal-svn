# SCons script for librascal.
# vim: ts=4 sts=4 sw=4 et nocindent syntax=python
# $Id$

import os
import re

# Lists files matching a regexp (r) and located
# within the listed fs nodes (at).
def list(at, r):
    def listx(at, r):
        nodes = []
        for location in at:
            for file in os.listdir(location):
                fname = os.path.join(location, file)
                if os.path.isfile(fname):
                    if r.match(file):
                        nodes.append(fname)
                elif os.path.isdir(fname):
                    if not os.path.basename(fname) == '.svn':
                        for more in listx([fname], r):
                            nodes.append(more)
        return nodes
    l = listx(at, re.compile(r))
    l.sort()
    return l

# Fix the construction environment: debug builds etc.
def fixenv(xenv, kw):
    env = xenv.Copy()
    for k, v in kw.items():
        env[k].append(v)
    if env['debug'] == 1:
        env.Append(CPPDEFINES = '_DEBUG')
        if env['CC'] == 'cl':
            env.Append(CCFLAGS = '/Od /GX /EHsc /MDd /Z7')
            env.Append(LINKFLAGS = '/DEBUG /OPT:NOICF /OPT:NOREF')
        elif env['CC'] == 'gcc':
            env.Append(CCFLAGS = '-g -O0')
    else:
        if env['CC'] == 'cl':
            env.Append(CCFLAGS = '/Ox /G5 /GF /Gy /MD')
            env.Append(LINKFLAGS = '/RELEASE /OPT:REF /OPT:ICF')
        elif env['CC'] == 'gcc':
            env.Append(CCFLAGS = '-O3 -fomit-frame-pointer -finline-functions')
            env.Append(LINKFLAGS = '--gc-sections')
    return env

# Build the list of source/object file name pairs.
def matchso(env, paths, mask):
    items = []
    for item in list(paths, mask):
        fname, ext = os.path.splitext(item)
        if env['debug'] == 1:
            fname = fname + '~debug'
        items.append((item, fname + '~shared.o'))
    return items

# Build a shared library, fixing object file names.
def SharedLibrary(xenv, name, paths, mask, **kw):
    obj = []
    env = fixenv(xenv, kw)
    for s, o in matchso(env, paths, mask):
        obj.append(env.SharedObject(o, s))
    lib = env.SharedLibrary(name, obj)
    env.Alias('install', env['instdir'])
    env.Install(env['instdir'] + '/lib', lib)
