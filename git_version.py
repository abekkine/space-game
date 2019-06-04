import subprocess
import re

gitVersion = True
try:
    labelStr = subprocess.check_output(["git", "describe", "--tags", "--always", "--dirty=-M"])
    label = labelStr.strip().split('-')
except Exception, e:
    gitVersion = False

if gitVersion:

    length = len(label)
    
    m = re.search('[vV][0-9]*\.[0-9]*$', label[0])
    
    strictForm = True
    if m != None:
        # Version label in format Vx.y
        vLabel = m.group(0)
    else:
        m = re.search('[vV][0-9]*\.[0-9]*\.[0-9]*$', label[0])
        if m != None:
            # Version label in format vX.y.Z
            vLabel = m.group(0)
        else:
            # Version label in other form.
            strictForm = False
            vLabel = label[0]
    
    if strictForm:
        # "vx.y[.z].n (12ab) "
        if length == 1:
            # Name
            VersionStr = "%s  " % vLabel
        elif length == 2:
            # Name + Modified
            VersionStr = "%s !" % vLabel
        elif length == 3:
            # Name + CommitCount + Hash
            VersionStr = "%s.%s (%s) " % (vLabel, label[1], label[2][1:5])
        elif length == 4:
            # Name + CommitCount + Hash + Modified
            VersionStr = "%s.%s (%s)!" % (vLabel, label[1], label[2][1:5])
        else:
         	VersionStr = labelStr.strip()
    
    else:
        # Print as it is.
        VersionStr = labelStr.strip()

else:
    VersionStr = 'N/A'


header_lines = [
 '#ifndef VERSION_H_\n',
 '#define VERSION_H_\n',
 '#define VERSION_STR "%s"\n' % VersionStr,
 '#endif // VERSION_H_\n'
]

with open('version.h', 'wt') as f:
    f.writelines(header_lines)


