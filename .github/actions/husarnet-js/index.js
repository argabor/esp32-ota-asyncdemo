const core = require("@actions/core");
const github = require("@actions/github");
const exec = require("@actions/exec");
const cache = require("@actions/cache");

// most @actions toolkit packages have async methods
async function run() {
  try {

    const joincode = core.getInput("join-code");
    const hostname = core.getInput("hostname");

    let myOutput = "";
    let myError = "";

    const options = {};
    options.listeners = {
      stdout: (data) => {
        myOutput = data.toString();
      },
      stderr: (data) => {
        myError = data.toString();
      },
    };

    // const paths = [
    //   // `${github.}`,
    //   "packages/*/node_modules/",
    // ];
    // const key = "npm-foobar-d5ea0750";
    // const cacheId = await cache.saveCache(paths, key);

    // https://github.com/actions/toolkit/issues/346
    await exec.exec(
      `/bin/bash -c "curl https://install.husarnet.com/install.sh | sudo bash"`
    );
    await exec.exec(`/bin/bash -c "sudo systemctl restart husarnet"`);

    console.log("Waiting for Husarnet to be ready");
    await exec.exec(`/bin/bash -c "while [ $(sudo husarnet status | grep "ERROR" | wc -l) -gt 0 ]; do echo .; sleep 1; done"`);
    console.log("done");

    await exec.exec(
      `/bin/bash -c "sudo husarnet status"`,
      options
    );
    console.log(myOutput.toString());

    await exec.exec(
      `/bin/bash -c "sudo husarnet join ${joincode} ${hostname}"`,
      options
    );
    console.log("output is:" + myOutput.toString());

    
    console.log(`JoinCode of this GA: ${joincode}`);

    const ipv6 = "fc94:2283:b56b:beeb:xxxx:xxxx:xxxx:xxxx";
    core.setOutput("ipv6", ipv6);

    console.log(JSON.stringify(github, null, "\t"));
  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
