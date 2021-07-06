const path = require('path');
const fs = require('fs');
const shell = require('shelljs');
const rimraf = require('rimraf');

// https://github.com/shelljs/shelljs/issues/469
process.env.PATH += (path.delimiter + path.join(process.cwd(), 'node_modules', '.bin'));

const PROTO_DIR = path.join(__dirname, '../proto');
const OUT_DIR = path.join(__dirname, '../include/proto');
const PROTOC_GEN_TS_PATH = path.join(__dirname, '../node_modules/.bin/protoc-gen-ts.cmd');

if (fs.existsSync(OUT_DIR)) {
  rimraf.sync(`${OUT_DIR}/*`);
} else {
  fs.mkdirSync(OUT_DIR);
}

const relativeProtoFiles = shell.exec(`find ./proto -name "*.proto"`, {silent:true}).stdout;

let absProtoFiles = [];
relativeProtoFiles.split('\n').forEach(file => {
  if (file.length !== 0) {
    absProtoFiles.push(path.join(__dirname, '../', file));
  }
})

console.log(absProtoFiles);

const protoConfig = [
  `--plugin="protoc-gen-ts=${PROTOC_GEN_TS_PATH}" `,
  `--cpp_out=${OUT_DIR}`,
  `--grpc_out="grpc_js:${OUT_DIR}" `,
  `--js_out="import_style=commonjs,binary:${OUT_DIR}" `,
  `--ts_out="grpc_js:${OUT_DIR}" `,
  `--proto_path ${PROTO_DIR} ${absProtoFiles.join(' ')}`
];

// https://github.com/agreatfool/grpc_tools_node_protoc_ts/tree/master/examples
shell.exec(`grpc_tools_node_protoc ${protoConfig.join(' ')}`);

// https://github.com/dcodeIO/protobuf.js#command-line
// https://github.com/dcodeIO/protobuf.js#command-line-api
