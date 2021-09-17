```bash
yarn add @actions/github
yarn add @actions/core
```

## bundling into 1 file

```bash
yarn add @vercel/ncc
npx ncc build .github/actions/husarnet-js/index.js -o .github/actions/husarnet-js/dist
```