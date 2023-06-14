# MotorLand

Project for Embedded Systems course (Spring 2023)

## Project object diagram
[View link](https://drive.google.com/file/d/1G1Tt3iVF1fO9oixSPcnYtCqgEOOCVNLW/view)

![](docs/img/project_object_diagram.png)

## Development in Clion
1. Choose module: `config_manager`, `daemon`, `gpiolib` or `state_manager`
2. Run `Reset Cache and Reload Project` action in Clion
3. Click on build button (hammer)
    - This will download all dependencies, compile and link them
    - Then it build the final executable. You can find it in `cmake-build-debug/bin/`