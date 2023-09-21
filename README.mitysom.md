This repository is an example/template and guide to building embedded Linux
images for the CriticalLink MitySOM series of System-on-Module boards based
on the Intel CycloneV FPGA programmable System-on-Chip device. 

## Dependencies & Upstream Projects
* CriticalLink [MitySOM5-cs](https://www.criticallink.com/product/mitysom-5csx/)
* [VSCode IDE](https://code.visualstudio.com/)
* [Docker/DockerDesktop](https://www.docker.com/)
* [devcontainers](https://containers.dev/)
* [YoctoProject](https://www.yoctoproject.org/)
* [meta-intel-fpga](https://git.yoctoproject.org/meta-intel-fpga)
* [Intel FPGA open-source support](https://www.rocketboards.org)

## Structure

We use devcontainers to encapsulate the built tools so that your VSCode IDE
may run on MacOS/Windows (Docker required), and a number of automation helpers
to make building runtime images from the VSCode IDE easier. See [here](doc/00-QuickStart.md) for getting starting, and for gory details on the Yocto
specific concepts. For further information about things SOCFPGA see the
RocketBoards pages. For specific guidance about things related to the MitySOM see [here](doc/mitysom/README.md).