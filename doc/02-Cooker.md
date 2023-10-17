# Cook what?
The [Yocto Project](https://www.yoctoproject.org) tools require downloading
both the tool-chain (bitbake) as well as a collection of supporting meta-data
repositories (layers), and then constructing a local configuration file which
specifies what options to use from the layers. To automate this process we use
a python tool named [cooker](https://github.com/cpb-/yocto-cooker).
