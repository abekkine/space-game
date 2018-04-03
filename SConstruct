import os
import platform

bits, opsys = platform.architecture()
if opsys[:3] == 'Win':
    commandScript = 'version.cmd'
    includePath = 'C:/Source/Libs'
else:
    commandScript = './version.sh'
    includePath = '/usr/include'

TARGET_NAME = 'spaceGame'

# env = Environment(ENV = {'PATH' : os.environ['PATH']})
# env.Replace(CXX = 'g++')
env = Environment(tools=['mingw'], ENV = {'PATH' : os.environ['PATH']})


files = []
files.append( Glob( '*.cpp' ) )

clean_list = []
clean_list.append( './sconsign.dblite' )
clean_list.append( 'core*' )
clean_list.append( 'version.h' )
env.Clean( 'default', clean_list )

env.Append( CPPPATH = [ '.' ] )
env.Append( CPPPATH = [ './3rd-party' ] )
# env.Append( CPPPATH = [ '%s/SDL' % includePath ] )
# env.Append( CPPPATH = [ '%s/freetype2' % includePath ] )
env.Append( CPPPATH = [ '%s/glfw/include' % includePath ] )
env.Append( CPPPATH = [ '%s/ftgl/include' % includePath ] )
env.Append( CPPPATH = [ '%s/freetype2/include' % includePath ] )
env.Append( CPPPATH = [ '%s/freetype2/include/freetype2' % includePath ] )
# env.Append( CPPPATH = [ '%s/SDL/include' % includePath ] )
# env.Append( CPPPATH = [ '%s' % includePath ] )
# env.Append( CPPPATH = [ '%s' % includePath ] )
env.Append( CPPPATH = [ '%s/Box2D' % includePath ] )

env.Append( CPPFLAGS = [ '-g' ] )
env.Append( CPPFLAGS = [ '--std=c++11' ] )
# env.Append( CPPFLAGS = [ '-Wall' ] )
# env.Append( CPPFLAGS = [ '-Wextra' ] )
# env.Append( CPPFLAGS = [ '-Werror' ] )
# env.Append( CPPFLAGS = [ '-Wfatal-errors' ] )

env.Append( LIBS = [ 'glfw' ] )
env.Append( LIBS = [ 'GL' ] )
# env.Append( LIBS = [ 'SDL' ] )
# env.Append( LIBS = [ 'SDL_image' ] )
env.Append( LIBS = [ 'ftgl' ] )
env.Append( LIBS = [ 'pthread' ] )
env.Append( LIBS = [ 'Box2D' ] )

versionCmd = env.Command( 'version.h', '', commandScript )
env.Depends( 'menu.h', versionCmd )

env.Program( TARGET_NAME, source = files )

