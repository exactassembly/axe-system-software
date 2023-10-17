# Devcontainer structure
The exact format of the files used to control spinup of the Docker container
can be found in the [devcontainers specification](https://containers.dev/), but
it can be useful to understand why these components are used.

## Dockerfile
Since the Yocto/Openembedded bitbake tool is written in Python, we use the
community maintained Python:3 Docker image as a baseline. There are a few
additional core tools required on the 'host' to run bitbake, so we add these
apt packages as well.

## docker-compose.yaml
Yocto builds everything used in the generation of a target image from source,
that includes all of the host-executables (aka 'native'), target libraries,
and all of the target exe
We use docker-compose to construct both the core container (from the Dockerfile)
with necessary mounted /workspace filesystem (to access the project sources).

## devcontainer-features
In order to allow for a devcontainer to provide a rich set of functionality,
such as mixing multiple types of build tools or support for multiple programming
languages, the devcontainer specification provides a way to add to a container
after the Dockerfile/docker-compose step is completed.

For this project we add a single devcontainer-feature "yocto-cooker" which
installs the automation tool 'cooker' in our standard path.

## vscode extras
When opening a devcontainer in VSCode, if a .vscode folder exists, the contents
will be used to install VSCode extensions, and populate the "Run Task..." menu. 
For this project we provide a number of helpful tasks that will correctly configure
the cooker tool, and cause a valid Linux target image to be generated. 