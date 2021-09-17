const core = require("@actions/core");
const github = require("@actions/github");
const exec = require("@actions/exec");



// most @actions toolkit packages have async methods
async function run() {
  try {
    // https://github.com/actions/toolkit/issues/346
    await exec.exec(`/bin/bash -c "curl https://install.husarnet.com/install.sh | sudo bash"`);

    const joincode = core.getInput("join-code");
    console.log(`JoinCode of this GA: ${joincode}`);

    const ipv6 = "fc94:2283:b56b:beeb:xxxx:xxxx:xxxx:xxxx";
    core.setOutput("ipv6", ipv6);

    console.log(JSON.stringify(github, null, "\t"));
  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
