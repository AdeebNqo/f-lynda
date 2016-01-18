import os,sys
import magic

path = '/tmp/flynda'
if len(sys.argv) > 1:
	path = sys.argv[1]
filelist = os.listdir(path)
for currfile in filelist:
	fullfilepath = '{1}{0}'.format(currfile, path)
	filetype = magic.from_file(fullfilepath, mime=True)
	if (filetype == 'audio/mpeg'):
		name, ext = os.path.splitext(fullfilepath)
		if len(ext)!=3:
			os.rename(fullfilepath, fullfilepath+'.mp3')
