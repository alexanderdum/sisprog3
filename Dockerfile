FROM ubuntu
RUN apt-get update -y && apt-get install build-essential nano gdb gcc-multilib -y
WORKDIR /wrkdir
CMD ["/bin/bash"]
