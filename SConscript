from building import *
import rtconfig

cwd     = GetCurrentDir()
src     = Glob('src/*.c')
path    = [cwd + '/inc']


src += Glob('samples/ssd1306/*.c')
path += [cwd + '/samples/ssd1306']

group = DefineGroup('rtfb', src, depend = [''], CPPPATH = path)

Return('group')
