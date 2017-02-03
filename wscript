# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding:
# utf-8; -*-
# © 2010 - 2016, Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V. All rights reserved.
#
# BSD 3-Clause License
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
# 1.  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
# 3.  Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# We kindly request you to use one or more of the following phrases to refer to foxBMS in your hardware, software, documentation or advertising materials:
#
# 'This product uses parts of foxBMS®'
#
# 'This product includes parts of foxBMS®'
#
# 'This product is derived from foxBMS®'

from waflib import Logs, Utils, Context, Options
from waflib import Task, TaskGen
import os
import sys
import datetime
import json
import platform
import subprocess

VERSION = "0.4.3"
APPNAME = "foxbms"
VENDOR = 'Fraunhofer IISB'
LDSCRIPT = 'src/STM32F429ZIT6_FLASH.ld'
SSCRIPT = 'src/general/config/startup_stm32f429xx.S'

def options(opt):
    opt.load('compiler_c')
    opt.load(['doxygen', 'sphinx_build'], tooldir=['tools/waftools'])

    #opt.load(['doxygen', 'sphinx_build'], tooldir=['.waf-tools'])
    #opt.add_option('-u', '--use', dest='use', default=False, action='store_true', help='a boolean option')
    opt.add_option('-c', '--config', action='store', default=None, help='file containing additional configuration variables', dest='configfile')
    opt.add_option('-t', '--target', action='store', default='debug', help='build target: debug (default)/release', dest='target')
    opt.add_option('--enable-latex', action='store_true',  default=False, help='enable latex->pdf generation of quickstart guide', dest='enable_latex')

def loadConfigFile():
    _fname = Options.options.configfile
    if _fname is None:
        return
    json.load(_fname)

def configure(conf):

    loadConfigFile()

    # store default env reference
    #conf.setenv('release')

    #conf.load(['doxygen', 'sphinx_build'])

    pref = 'arm-none-eabi'

    if sys.platform.startswith('win'):
        conf.env.CC = pref + '-gcc.exe'
        conf.env.AR = pref + '-ar.exe'
        conf.find_program(pref + '-strip', var='STRIP')
        conf.env.LINK_CC = pref + '-g++.exe'
        conf.find_program(pref + '-objcopy', var='hexgen')
        conf.find_program(pref + '-size', var='SIZE')
    else:
        conf.env.CC = pref + '-gcc'
        conf.env.AR = pref + '-ar'
        conf.env.LINK_CC = pref + '-g++'
        conf.find_program(pref + '-strip', var='STRIP')
        conf.find_program(pref + '-objcopy', var='hexgen')
        conf.find_program(pref + '-size', var='SIZE')
    conf.env.CFLAGS = '-mthumb -mlittle-endian -fsigned-char -ffreestanding -fno-move-loop-invariants -fmessage-length=0 -fsigned-char -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=c99 -ffunction-sections -fdata-sections -Wall'.split(' ')
    conf.env.CFLAGS += '-DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DUSE_HAL_DRIVER -DHSE_VALUE=8000000'.split(' ')
    # -std=gnu90
    conf.load('compiler_c')
    conf.load(['doxygen', 'sphinx_build'])
    if conf.options.enable_latex:
        conf.load('tex')                                                                                                                                             
        if not conf.env.LATEX:                                                                                                                                       
            conf.fatal('The program LaTeX is required')    

    #conf.load('strip', tooldir='tools/waftools')
    conf.find_program('git', mandatory = False)


    conf.env.version = VERSION
    conf.env.appname = APPNAME
    conf.env.vendor = VENDOR
    conf.env.ldscript = os.path.join(conf.srcnode.abspath(), LDSCRIPT)
    conf.env.sscript = os.path.join(conf.srcnode.abspath(), SSCRIPT)
    try:
        conf.env.buildno = conf.cmd_and_log(conf.env.GIT[0] + ' rev-parse --short HEAD').strip()
    except:
        conf.env.buildno = 'none'
    utcnow = datetime.datetime.utcnow()
    utcnow = ''.join(utcnow.isoformat('-').split('.')[0].replace(':', '-').split('-'))
    conf.env.timestamp = utcnow

    conf.env.generalincludes = ' '.join([
        os.path.join(conf.bldnode.abspath()),
        os.path.join(conf.srcnode.abspath(), 'src', 'engine', 'config'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'engine', 'database'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'engine', 'task'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'engine', 'sysctrl'),
        os.path.join(conf.srcnode.abspath(), 'src', 'general'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'general', 'config'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'general', 'includes'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'hal', 'CMSIS', 'Device', 'ST', 'STM32F4xx', 'Include'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'hal', 'CMSIS', 'Include'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'hal', 'STM32F4xx_HAL_Driver', 'Inc'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'hal', 'STM32F4xx_HAL_Driver', 'Inc', 'Legacy'),
        os.path.join(conf.srcnode.abspath(), 'src', 'module', 'cpu'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'module', 'io'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'module', 'config'),
        os.path.join(conf.srcnode.abspath(), 'src', 'module', 'contactor'),
        os.path.join(conf.srcnode.abspath(), 'src', 'module', 'rtc'),
        os.path.join(conf.srcnode.abspath(), 'src', 'engine', 'diag'),
        os.path.join(conf.srcnode.abspath(), 'src', 'module', 'eeprom'),
        os.path.join(conf.srcnode.abspath(), 'src', 'os'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'os', 'FreeRTOS', 'Source', 'CMSIS_RTOS'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'os', 'FreeRTOS', 'Source', 'include'), 
        os.path.join(conf.srcnode.abspath(), 'src', 'os', 'FreeRTOS', 'Source', 'portable', 'GCC', 'ARM_CM4F'), 
        ])


    for k in 'gcc ar cpp ranlib as'.split():
        print pref + '-' + k, k.upper()
        conf.find_program(pref + '-' + k, var=k.upper(), mandatory = True)

    conf.define('BUILD_APPNAME', APPNAME)
    conf.define('BUILD_VERSION', VERSION)
    conf.define('BUILD_VENDOR', VENDOR)
    conf.define('BUILD_LDSCRIPT', conf.env.ldscript)
    conf.define('BUILD_NUMBER', conf.env.buildno)
    conf.define('TOOLCHAIN_WAF_ENABLED', 1)
    conf.define('STM32F429xx', 1)
    conf.define('USE_DRIVER_HAL', 1)
    conf.define('INCLUDE_eTaskGetState', 1)

    conf.env.target = conf.options.target
    #conf.env.EXT_CC += ['.c', '.cc', '.s', '.S']
    conf.env.EXT_CC += ['.S']

    
    env_debug = conf.env.derive()
    env_debug.detach()
    env_release = conf.env.derive()
    env_release.detach()

    conf.setenv('debug', env_debug)
    conf.define('RELEASE', 1)
    conf.undefine('DEBUG')
    conf.env.CFLAGS += ['-g', '-O0']
    env_debug.basename = conf.env.appname + '-' + conf.env.version + '-' + conf.env.buildno + '-' + conf.env.timestamp + '-debug'
    env_debug.PREFIX = conf.env.appname + '-' + conf.env.version + '-' + conf.env.buildno + '-' + conf.env.timestamp

    conf.setenv('release', env_release)
    conf.env.CFLAGS += ['-O2']
    env_release.basename = conf.env.appname + '-' + conf.env.version + '-' + conf.env.buildno + '-' + conf.env.timestamp + '-release'
    env_release.PREFIX = conf.env.appname + '-' + conf.env.version + '-' + conf.env.buildno + '-' + conf.env.timestamp

    if conf.options.target == 'release':
        conf.setenv('', env_release)
    else:
        conf.setenv('', env_debug)

    env_release.store('env.txt')
    #env_debug.store('debug_env.txt')

    conf.write_config_header('foxbmsconfig.h')
    print conf.env.basename, conf.env.CFLAGS, conf.env.PREFIX


    #conf.env.CFLAGS = '-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=c99 -O0 -ffunction-sections -fdata-sections -g -Wall'.split(' ')
    #conf.check_cc(fragment='int main(){return 0;}')
    #conf.check_tool('gcc gas')


def build(bld):

    import sys, logging
    from waflib import Logs
    bld.logger = Logs.make_logger('log.txt', 'build')
    hdlr = logging.StreamHandler(sys.stdout)
    formatter = logging.Formatter('%(message)s')
    hdlr.setFormatter(formatter)
    bld.logger.addHandler(hdlr)

    bld.recurse('src')

def dist(conf):
    conf.base_name = 'foxbms' 
    conf.algo      = 'tar.gz' 
    conf.excl      = ' **/.waf-1* **/*~ **/*.pyc **/*.swp **/.lock-w* **/env.txt **/log.txt **/.git **/build **/*.tar.gz **/.gitignore **/tools/waf-1.8.12-*' 
        

def chksum(bld):
    subprocess.Popen("python tools/checksum/ChkSum.py tools/checksum/chksum.ini", shell=True)

def install(bld):
    bld.env.MESSAGE = 'hhh'

def flash(bld):
    #subprocess.call("python tools/flashtool/stm32_loader_reset.py -p COM3 -e -w -v -a 0x40020000 "+ bld.path.abspath() + ("/build/src/general/foxbms_bkpram.bin"), shell=True)
    subprocess.call("python tools/flashtool/stm32_loader.py -p COM10 -e -w -v -a 0x08000000 "+ bld.path.abspath() + ("/build/src/general/foxbms_flash.bin"), shell=True)
    subprocess.call("python tools/flashtool/stm32_loader.py -p COM10 -w -v -a 0x080FFF00 "+ bld.path.abspath() + ("/build/src/general/foxbms_flashheader.bin"), shell=True)

def cpplint(bld):

    subprocess.Popen("python tools/cpplint/checkall.py", shell=True)

def doxygen(bld):
    if bld.env.DOXYGEN:
        _docbuilddir = os.path.join(bld.bldnode.abspath(), 'doc')
        if not os.path.exists(_docbuilddir):
            os.makedirs(_docbuilddir)
        bld(features="doxygen", doxyfile='doc/doxygen/doxygen.conf')

def sphinx(bld):
    bld.recurse('doc/sphinx')
    
def size(bld):
    objlist = ''
    for root, dirs, files in os.walk("build"):
        for file in files:
            if file.endswith(".o"):
                
                print(os.path.join(root, file))
                objlist = objlist + " " + os.path.join(root, file)
    
    subprocess.Popen( "arm-none-eabi-size --format=berkeley build/src/general/foxbms.elf " + objlist , shell=True)

class strip(Task.Task):
    run_str = '${STRIP} ${SRC}'
    color   = 'BLUE'
    #after   = ['cprogram', 'cxxprogram', 'cshlib', 'cxxshlib', 'fcprogram', 'fcshlib']

@TaskGen.feature('strip')
@TaskGen.after('apply_link')
def add_strip_task(self):
    try:
        link_task = self.link_task
    except AttributeError:
        return
    self.create_task('strip', link_task.outputs[0])

class hexgen(Task.Task):
    run_str = '${hexgen} -O ihex ${SRC} src/general/foxbms.hex'
    color   = 'CYAN'
    #after   = ['cprogram', 'cxxprogram', 'cshlib', 'cxxshlib', 'fcprogram', 'fcshlib']

@TaskGen.feature('hexgen')
@TaskGen.after('apply_link')
def add_hexgen_task(self):
    try:
        link_task = self.link_task
    except AttributeError:
        return
    self.create_task('hexgen', link_task.outputs[0])

class binbkpramgen(Task.Task):
    run_str = '${hexgen} -j .bkp_ramsect -O binary ${SRC} src/general/foxbms_bkpram.bin'
    color   = 'RED'
    #after   = ['cprogram', 'cxxprogram', 'cshlib', 'cxxshlib', 'fcprogram', 'fcshlib']
class binflashheadergen(Task.Task):
    run_str = '${hexgen} -j .flashheader -O binary ${SRC} src/general/foxbms_flashheader.bin'
    color   = 'RED'
    #after   = ['cprogram', 'cxxprogram', 'cshlib', 'cxxshlib', 'fcprogram', 'fcshlib']
class binflashgen(Task.Task):
    run_str = '${hexgen} -R .bkp_ramsect -R .flashheader -O binary ${SRC} src/general/foxbms_flash.bin'
    color   = 'RED'
    #after   = ['cprogram', 'cxxprogram', 'cshlib', 'cxxshlib', 'fcprogram', 'fcshlib']


@TaskGen.feature('bingen')
@TaskGen.after('apply_link')
def add_bingen_task(self):
    try:
        link_task = self.link_task
    except AttributeError:
        return
    self.create_task('binbkpramgen', link_task.outputs[0])
    self.create_task('binflashgen', link_task.outputs[0])
    self.create_task('binflashheadergen', link_task.outputs[0])

import waflib.Tools.asm # import before redefining
from waflib.TaskGen import extension
@extension('.S')
def asm_hook(self, node):
    name = 'Sasm'
    out = node.change_ext('.o')
    task = self.create_task(name, node, out)
    try:
                self.compiled_tasks.append(task)
    except AttributeError:
                self.compiled_tasks = [task]
    return task

class Sasm(Task.Task):
    color = 'BLUE'
    run_str = '${CC} ${CFLAGS} ${CPPPATH_ST:INCPATHS} -DHSE_VALUE=8000000 -MMD -MP -MT${TGT} -c -x assembler -o ${TGT} ${SRC}'
#-DHSE_VALUE=8000000 -I"D:\foxBMSSandboxPush\EclipseWorkspace?\IISBlibrary" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o

# vim: set ft=python : 



