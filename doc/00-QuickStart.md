# Getting Started

## ;tldr;

To generate the default image in this repository, open a terminal in the devcontainer and run:

> /opt/cooker/scripts/broiler.sh broil

Alternatively use the provided VSCode tasks (Terminal->Run Task...) which
will perform the necessary cooker initialization, download, and build steps.

## Running Locally

### Installation Requirements

- Git
- Docker/docker-compose/docker desktop
- VSCode
  - VSCode extension: dev containers

### Starting the Devcontainer

Note: Ensure that Docker-Desktop is running before continuing

#### Option 1: Running from a local clone

1. Clone git repository into a local folder
2. In VSCode: open cloned repository in VSCode (File->open folder...)
3. In VSCode: open the Command Palette (View->Command Palette...) and run: `Dev Containers: Reopen in Container`

#### Option 2: Cloning into a container volume

To Do

### Building the image

To generate the image in this repository, open a terminal in the devcontainer and run:
> /opt/cooker/scripts/broiler.sh broil

Alternatively, use the provided VSCode build task (Terminal->Run Build Task...).

## What is this stuff?

This repo contains boilerplate structure necessary to start a devcontainer
environment in Visual Studio Code, download the necessary Yocto Project sources
to temporary space, configure the Yocto/OpenEmbedded bitbake tools, and generate
a runnable Yocto runtime.

### Devcontainer?

There are nearly infinite numbers of programming languages from C to Rust, and
many more build orchestration tools from classic Makefile to CMake, which
attempt to manage the complexities of system libraries, build configuration and
versioning. While these often try to be 'universal', there can be OS and version
dependencies for the very tools used, a typical solution is the Python 'pyenv' too
which allows multiple versions of Python to exist in parallel.

For developers who may have multiple projects ongoing even managing pyenv is too
much. A common modern solution for all of the dependencies of a project is to use Docker containers to hold both the "host" tools and the target development code.
Microsoft introduced an open-source specification [devcontainers](https://containers.dev/) with an open-source reference implementation
[devcontainers-cli](https://github.com/devcontainers/cli), which is part of the VSCode IDE by default, and also supported as a web based IDE through GitHub.

### Yocto Project?
The [Yocto Project](https://www.yoctoproject.org) is a set of tools (bitbake) and
metadata (layers) for building embedded Linux runtimes (kernel, filesystem,
bootloader, etc.). It is supported by most major CPUs, and has a robust ecosystem
of partners.