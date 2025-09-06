project(
    'cz-core',
    'c','cpp',
    version : run_command('cat', files('VERSION'), check : false).stdout().strip(),
    meson_version: '>= 0.62.0',
    default_options: [
        'warning_level=2',
        'buildtype=release',
        'cpp_std=c++20'])

VERSION_MAJOR = meson.project_version().split('.')[0]
VERSION_MINOR = meson.project_version().split('.')[1]
VERSION_PATCH = meson.project_version().split('.')[2]
VERSION_BUILD = run_command('cat', './BUILD', check : false).stdout()

# -------------- CONFIGURATION --------------

HEADERS_INSTALL_PATH = join_paths(get_option('prefix'), get_option('includedir'))
ASSETS_INSTALL_PATH = join_paths(get_option('prefix'), get_option('datadir'), 'cz/core/assets')

add_project_arguments([
    '-DCZ_CORE_VERSION_MAJOR=@0@'.format(VERSION_MAJOR.to_int()),
    '-DCZ_CORE_VERSION_MINOR=@0@'.format(VERSION_MINOR.to_int()),
    '-DCZ_CORE_VERSION_PATCH=@0@'.format(VERSION_PATCH.to_int()),
    '-DCZ_CORE_VERSION_BUILD=@0@'.format(VERSION_BUILD.to_int()),
    '-DCZ_CORE_DEFAULT_ASSETS_PATH="@0@"'.format(ASSETS_INSTALL_PATH)
], language: 'cpp')

# -------------- DEPENDENCIES --------------

cpp             = meson.get_compiler('cpp')
pkg             = import('pkgconfig')
deps = [
    dependency('cz-skia'),
    dependency('xkbcommon')
]

# -------------- SOURCES --------------

include_paths = [include_directories('src/')]

# All headers
headers = run_command('find', 'src/CZ', '-type', 'f', '-name', '*.h', check : false).stdout().strip().split('\n')

# Public and private API headers
header_dirs = [
    ['src/CZ/Core', 'CZ/Core'],
    ['src/CZ/Events', 'CZ/Events'],
    ['src/CZ/Utils', 'CZ/Utils']
]

foreach header_dir : header_dirs
    header_files = run_command('find', header_dir[0], '-type', 'f', '-name', '*.h', '-maxdepth', '1', check : false).stdout().strip().split('\n')
    install_headers(header_files, install_dir : join_paths(HEADERS_INSTALL_PATH, header_dir[1]))
    include_paths += include_directories(header_dir[0])
endforeach
# -------------- LIBRARY --------------

cz_core = library(
    'cz-core',
    sources : run_command('find', 'src/CZ', '-type', 'f', '-name', '*.cpp', check : false).stdout().strip().split('\n'),
    include_directories : [include_directories('src')],
    dependencies : deps,
    soversion: VERSION_MAJOR,
    install : true)

cz_core_dep = declare_dependency(
    dependencies: [],
    include_directories : [],
    link_with : cz_core)

pkg.generate(
    cz_core,
    name: 'cz-core',
    description: 'Cuarzo core C++ library.',
    version: meson.project_version(),
    requires: [])
