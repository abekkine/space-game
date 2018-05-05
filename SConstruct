TARGET_NAME = 'spaceGame'

env = Environment()

files = []
files.append( Glob( '*.cpp' ) )

clean_list = []
clean_list.append( './sconsign.dblite' )
clean_list.append( 'core*' )
clean_list.append( 'version.h' )
env.Clean( 'default', clean_list )

env.Append( CPPPATH = [ '.' ] )
env.Append( CPPPATH = [ '..' ] )
env.Append( CPPPATH = [ './3rd-party' ] )
env.Append( CPPPATH = [ '/usr/include/SDL' ] )
env.Append( CPPPATH = [ '/usr/include/freetype2' ] )
env.Append( CPPPATH = [ '/usr/include/Box2D' ] )

env.Append( CPPFLAGS = [ '-g' ] )
env.Append( CPPFLAGS = [ '-Wall' ] )
env.Append( CPPFLAGS = [ '-Wextra' ] )
env.Append( CPPFLAGS = [ '-Werror' ] )
env.Append( CPPFLAGS = [ '--std=c++14' ] )
env.Append( CPPFLAGS = [ '-Wfatal-errors' ] )

env.Append( LIBS = [ 'glfw' ] )
env.Append( LIBS = [ 'GL' ] )
env.Append( LIBS = [ 'SDL' ] )
env.Append( LIBS = [ 'SDL_image' ] )
env.Append( LIBS = [ 'ftgl' ] )
env.Append( LIBS = [ 'pthread' ] )
env.Append( LIBS = [ 'Box2D' ] )

versionCmd = env.Command( 'version.h', '', './version.sh' )
env.Depends( 'menu.h', versionCmd )

env.Program( TARGET_NAME, source = files )
