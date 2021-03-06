# This file is part of Open Evil Islands.
#
# Open Evil Islands is an open source, cross-platform port of the original Evil Islands from Nival.
# Copyright (C) 2009-2017 Yanis Kurganov <yanis.kurganov@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

TEMPLATE = app
TARGET = open-evil-islands

CONFIG += c++11 no_keywords

INCLUDEPATH = engine/headers build/resources

HEADERS = \
    engine/headers/aabb.hpp \
    engine/headers/adbfile.hpp \
    engine/headers/alloc.hpp \
    engine/headers/anmfile.hpp \
    engine/headers/anmstate.hpp \
    engine/headers/avcodec.hpp \
    engine/headers/bbox.hpp \
    engine/headers/bink.hpp \
    engine/headers/bitmanipulations.hpp \
    engine/headers/bonfile.hpp \
    engine/headers/byteorder.hpp \
    engine/headers/camera.hpp \
    engine/headers/camfile.hpp \
    engine/headers/color.hpp \
    engine/headers/complection.hpp \
    engine/headers/conditionvariable.hpp \
    engine/headers/configfile.hpp \
    engine/headers/configmanager.hpp \
    engine/headers/display.hpp \
    engine/headers/display_windows.hpp \
    engine/headers/display_x11.hpp \
    engine/headers/event.hpp \
    engine/headers/exception.hpp \
    engine/headers/figbone.hpp \
    engine/headers/figentity.hpp \
    engine/headers/figfile.hpp \
    engine/headers/fighelpers.hpp \
    engine/headers/figmesh.hpp \
    engine/headers/fignode.hpp \
    engine/headers/figproto.hpp \
    engine/headers/figrenderitem.hpp \
    engine/headers/figuremanager.hpp \
    engine/headers/font.hpp \
    engine/headers/fps.hpp \
    engine/headers/frustum.hpp \
    engine/headers/glew.hpp \
    engine/headers/glew_windows.hpp \
    engine/headers/glew_x11.hpp \
    engine/headers/graphicscontext.hpp \
    engine/headers/graphicscontext_windows.hpp \
    engine/headers/graphicscontext_x11.hpp \
    engine/headers/input.hpp \
    engine/headers/lnkfile.hpp \
    engine/headers/logging.hpp \
    engine/headers/makeunique.hpp \
    engine/headers/material.hpp \
    engine/headers/matrix3.hpp \
    engine/headers/matrix4.hpp \
    engine/headers/memfile.hpp \
    engine/headers/mmpfile.hpp \
    engine/headers/mobfile.hpp \
    engine/headers/mobloader.hpp \
    engine/headers/mobmanager.hpp \
    engine/headers/mprfile.hpp \
    engine/headers/mprhelpers.hpp \
    engine/headers/mprmanager.hpp \
    engine/headers/mprrenderitem.hpp \
    engine/headers/object.hpp \
    engine/headers/occlusion.hpp \
    engine/headers/opengl.hpp \
    engine/headers/optionmanager.hpp \
    engine/headers/optparse.hpp \
    engine/headers/plane.hpp \
    engine/headers/property.hpp \
    engine/headers/quaternion.hpp \
    engine/headers/ray.hpp \
    engine/headers/rectangle.hpp \
    engine/headers/regfile.hpp \
    engine/headers/registry.hpp \
    engine/headers/rendergroup.hpp \
    engine/headers/renderitem.hpp \
    engine/headers/renderlayer.hpp \
    engine/headers/renderqueue.hpp \
    engine/headers/rendersystem.hpp \
    engine/headers/renderwindow.hpp \
    engine/headers/resball.hpp \
    engine/headers/resfile.hpp \
    engine/headers/resourcemanager.hpp \
    engine/headers/ringbuffer.hpp \
    engine/headers/root.hpp \
    engine/headers/scenemanager.hpp \
    engine/headers/scenenode.hpp \
    engine/headers/segment.hpp \
    engine/headers/semaphore.hpp \
    engine/headers/shader.hpp \
    engine/headers/shadermanager.hpp \
    engine/headers/singleton.hpp \
    engine/headers/soundblock.hpp \
    engine/headers/soundbuffer.hpp \
    engine/headers/soundcapabilities.hpp \
    engine/headers/sounddevice.hpp \
    engine/headers/soundformat.hpp \
    engine/headers/soundinstance.hpp \
    engine/headers/soundmanager.hpp \
    engine/headers/soundmixer.hpp \
    engine/headers/soundobject.hpp \
    engine/headers/soundoptions.hpp \
    engine/headers/soundresource.hpp \
    engine/headers/soundsystem.hpp \
    engine/headers/sphere.hpp \
    engine/headers/string.hpp \
    engine/headers/systemevent.hpp \
    engine/headers/systeminfo.hpp \
    engine/headers/terrain.hpp \
    engine/headers/texture.hpp \
    engine/headers/texturemanager.hpp \
    engine/headers/thread.hpp \
    engine/headers/threadflag.hpp \
    engine/headers/threadlock.hpp \
    engine/headers/threadpool.hpp \
    engine/headers/timer.hpp \
    engine/headers/transform.hpp \
    engine/headers/triangle.hpp \
    engine/headers/untransferable.hpp \
    engine/headers/utility.hpp \
    engine/headers/value.hpp \
    engine/headers/vector.hpp \
    engine/headers/vector2.hpp \
    engine/headers/vector3.hpp \
    engine/headers/vector4.hpp \
    engine/headers/videobuffer.hpp \
    engine/headers/videoinstance.hpp \
    engine/headers/videomanager.hpp \
    engine/headers/videoobject.hpp \
    engine/headers/videooptions.hpp \
    engine/headers/videoresource.hpp \
    engine/headers/viewport.hpp \
    engine/headers/wave.hpp \
    engine/headers/ycbcr.hpp \
    tools/resfileviewer/sources/mainwindow.hpp \
    tools/resfileviewer/sources/treewidget.hpp

SOURCES = \
    engine/sources/aabb.cpp \
    engine/sources/adbfile.cpp \
    engine/sources/alloc.cpp \
    engine/sources/anmfile.cpp \
    engine/sources/anmstate.cpp \
    engine/sources/avcodec.cpp \
    engine/sources/bbox.cpp \
    engine/sources/bink.cpp \
    engine/sources/bonfile.cpp \
    engine/sources/byteorder.cpp \
    engine/sources/byteorder_generic.cpp \
    engine/sources/byteorder_linux.cpp \
    engine/sources/camera.cpp \
    engine/sources/camfile.cpp \
    engine/sources/color.cpp \
    engine/sources/complection.cpp \
    engine/sources/configfile.cpp \
    engine/sources/configmanager.cpp \
    engine/sources/display.cpp \
    engine/sources/display_windows.cpp \
    engine/sources/display_x11.cpp \
    engine/sources/event.cpp \
    engine/sources/figbone.cpp \
    engine/sources/figentity.cpp \
    engine/sources/figfile.cpp \
    engine/sources/fighelpers.cpp \
    engine/sources/figmesh.cpp \
    engine/sources/fignode.cpp \
    engine/sources/figproto.cpp \
    engine/sources/figrenderitem_opengl.cpp \
    engine/sources/figuremanager.cpp \
    engine/sources/font_opengl.cpp \
    engine/sources/fps.cpp \
    engine/sources/frustum.cpp \
    engine/sources/glew.cpp \
    engine/sources/glew_windows.cpp \
    engine/sources/glew_x11.cpp \
    engine/sources/graphicscontext.cpp \
    engine/sources/graphicscontext_windows.cpp \
    engine/sources/graphicscontext_x11.cpp \
    engine/sources/input.cpp \
    engine/sources/lnkfile.cpp \
    engine/sources/logging.cpp \
    engine/sources/material.cpp \
    engine/sources/matrix3.cpp \
    engine/sources/matrix4.cpp \
    engine/sources/memfile.cpp \
    engine/sources/mmpfile.cpp \
    engine/sources/mobfile.cpp \
    engine/sources/mobloader.cpp \
    engine/sources/mobmanager.cpp \
    engine/sources/mprfile.cpp \
    engine/sources/mprhelpers.cpp \
    engine/sources/mprmanager.cpp \
    engine/sources/mprrenderitem_opengl.cpp \
    engine/sources/object.cpp \
    engine/sources/occlusion_opengl.cpp \
    engine/sources/opengl.cpp \
    engine/sources/optionmanager.cpp \
    engine/sources/optparse.cpp \
    engine/sources/plane.cpp \
    engine/sources/property.cpp \
    engine/sources/quaternion.cpp \
    engine/sources/regfile.cpp \
    engine/sources/registry_generic.cpp \
    engine/sources/registry_linux.cpp \
    engine/sources/registry_windows.cpp \
    engine/sources/rendergroup.cpp \
    engine/sources/renderitem.cpp \
    engine/sources/renderlayer.cpp \
    engine/sources/renderqueue.cpp \
    engine/sources/rendersystem_opengl.cpp \
    engine/sources/renderwindow.cpp \
    engine/sources/renderwindow_windows.cpp \
    engine/sources/renderwindow_x11.cpp \
    engine/sources/resball.cpp \
    engine/sources/resfile.cpp \
    engine/sources/resourcemanager.cpp \
    engine/sources/root.cpp \
    engine/sources/scenemanager.cpp \
    engine/sources/scenenode.cpp \
    engine/sources/semaphore.cpp \
    engine/sources/shader_opengl.cpp \
    engine/sources/shadermanager.cpp \
    engine/sources/soundblock.cpp \
    engine/sources/soundbuffer.cpp \
    engine/sources/sounddevice.cpp \
    engine/sources/sounddevice_generic.cpp \
    engine/sources/sounddevice_linux.cpp \
    engine/sources/sounddevice_windows.cpp \
    engine/sources/soundinstance.cpp \
    engine/sources/soundmanager.cpp \
    engine/sources/soundmixer.cpp \
    engine/sources/soundobject.cpp \
    engine/sources/soundresource.cpp \
    engine/sources/soundresource_generic.cpp \
    engine/sources/soundsystem.cpp \
    engine/sources/sphere.cpp \
    engine/sources/string.cpp \
    engine/sources/systemevent.cpp \
    engine/sources/systeminfo_generic.cpp \
    engine/sources/systeminfo_posix.cpp \
    engine/sources/systeminfo_windows.cpp \
    engine/sources/terrain.cpp \
    engine/sources/texture_opengl.cpp \
    engine/sources/texturemanager.cpp \
    engine/sources/thread.cpp \
    engine/sources/thread_posix.cpp \
    engine/sources/thread_windows.cpp \
    engine/sources/threadpool.cpp \
    engine/sources/timer_generic.cpp \
    engine/sources/timer_posix.cpp \
    engine/sources/timer_windows.cpp \
    engine/sources/transform.cpp \
    engine/sources/triangle.cpp \
    engine/sources/utility.cpp \
    engine/sources/value.cpp \
    engine/sources/vector.cpp \
    engine/sources/vector2.cpp \
    engine/sources/vector3.cpp \
    engine/sources/videoinstance.cpp \
    engine/sources/videomanager.cpp \
    engine/sources/videoobject.cpp \
    engine/sources/videoresource.cpp \
    engine/sources/videoresource_generic.cpp \
    engine/sources/wave.cpp \
    spikes/figureviewer/main.cpp \
    spikes/mapviewer/main.cpp \
    spikes/mediaplayer/main.cpp \
    tools/reg2ini/main.c \
    tools/resfileviewer/sources/main.cpp \
    tools/resfileviewer/sources/mainwindow.cpp \
    tools/resfileviewer/sources/treewidget.cpp

HEADERS += build/resources/resource.hpp
SOURCES += build/resources/resource.cpp

DISTFILES += \
    AUTHORS \
    LICENSE \
    README \
    THANKS \
    SConstruct \
    SConscript \
    engine/SConscript \
    spikes/SConscript \
    tools/SConscript \
    tools/reg2ini/SConscript \
    tools/resfileviewer/SConscript \
    engine/resources/resource.res \
    spikes/figureviewer/resource.rcx \
    spikes/mapviewer/resource.rcx \
    spikes/mediaplayer/resource.rcx \
    open-evil-islands.cfg \
    open-evil-islands.cfg.user \
    open-evil-islands.cfg.user.example
