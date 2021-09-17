const core = require("@actions/core");
const github = require("@actions/github");
const exec = require('@actions/exec');

(async () => {
  try {
    await exec.exec('curl https://install.husarnet.com/install.sh | sudo bash');
  } catch (e) {
      // Deal with the fact the chain failed
  }
})();

try {
  
  const joincode = core.getInput("join-code");
  console.log(`JoinCode of this GA: ${joincode}`);

  const ipv6 = "fc94:2283:b56b:beeb:xxxx:xxxx:xxxx:xxxx";
  core.setOutput("ipv6", ipv6);

  console.log(JSON.stringify(github, null, "\t"));
} catch (error) {
  core.setFailed(error.message);
}
