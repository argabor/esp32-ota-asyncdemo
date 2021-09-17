const core = require("@actions/core");
const github = require("@actions/github");
const exec = require("@actions/exec");
const cache = require("@actions/cache");
const io = require('@actions/io');



// most @actions toolkit packages have async methods
async function run() {
  try {

    const joincode = core.getInput("join-code");
    const hostname = core.getInput("hostname");
    const cachekey = core.getInput("cache-key");

    let myOutput = "";
    let myError = "";

    // Recursive must be true for directories
    const options_io = { recursive: true, force: true }

    const options_exec = {};
    options_exec.listeners = {
      stdout: (data) => {
        myOutput = data.toString();
      },
      stderr: (data) => {
        myError = data.toString();
      },
    };

    const cache_paths = [
      './husarnet_cache',
    ]

    // const paths = [
    //   // `${github.}`,
    //   "packages/*/node_modules/",
    // ];
    // const key = "npm-foobar-d5ea0750";
    // const cacheId = await cache.saveCache(paths, key);
    
    console.log("starting:" + cache_paths[0]);

    // await io.mkdirP('./husarnet_cache');
    let result = await cache.restoreCache(cache_paths, cachekey);
    if ( result != undefined ) {
      await io.cp(cache_paths[0], '/var/lib/husarnet/', options_io);
    }

    // https://github.com/actions/toolkit/issues/346
    await exec.exec(
      `/bin/bash -c "curl https://install.husarnet.com/install.sh | sudo bash"`
    );

    // await io.cp('~/husarnet_cache', '/var/lib/husarnet/', options);

    await exec.exec(`/bin/bash -c "sudo systemctl restart husarnet"`);

    console.log("Waiting for Husarnet to be ready");
    await exec.exec(`/bin/bash -c "while [ $(sudo husarnet status | grep "ERROR" | wc -l) -gt 0 ]; do echo .; sleep 1; done"`);
    console.log("done");

    await exec.exec(
      `/bin/bash -c "sudo husarnet status"`,
      options_exec
    );
    console.log(myOutput.toString());

    await exec.exec(
      `/bin/bash -c "sudo husarnet join ${joincode} ${hostname}"`,
      options_exec
    );
    console.log("output is:" + myOutput.toString());

    await io.cp('/var/lib/husarnet/', cache_paths[0], options_io);
    await cache.saveCache(cache_paths, cachekey);

    console.log(`JoinCode of this GA: ${joincode}`);

    const ipv6 = "fc94:2283:b56b:beeb:xxxx:xxxx:xxxx:xxxx";
    core.setOutput("ipv6", ipv6);

    console.log(JSON.stringify(github, null, "\t"));


  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
