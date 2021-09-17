```bash
npm install @actions/github
npm install @actions/core
```

## bundling into 1 file

```bash
npm i -D @vercel/ncc
npx ncc build .github/actions/husarnet-js/index.js -o .github/actions/husarnet-js/dist
```