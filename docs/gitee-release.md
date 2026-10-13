# Gitee 推送自动发布 Windows 包

只推送 Gitee 的 `master` 即可触发，无需再手工同步推送 GitHub 或上传附件。Gitee Go 运行轻量触发步骤，GitHub Actions 的 Windows 云主机拉取该次 Gitee 提交，调用现有打包和启动检查脚本，最后通过 Gitee API 上传安装包、免安装 ZIP 和 `SHA256SUMS.txt`。

工作流固定发布仓库为 `xiaojiaz/EUI`，构建入口为 `lihechuan/EUI` 的 `gitee-release.yml`。它不会自动把 Gitee 源码推送到 GitHub 的公开分支；私有源码会在这个 GitHub 仓库的 Actions 主机上构建，Windows 包作为 Actions 构建产物保留 14 天。

## 一次性配置

1. 将 `.github/workflows/gitee-release.yml`、`packaging/gitee_release.py` 和 `packaging/test_gitee_release.py` 同步到 GitHub 的 `master`，用于提供构建入口。
2. 在 Gitee 创建具有 `projects` 权限、能够读取本仓库和管理发行版的私人令牌。在 GitHub 仓库的 **Settings → Secrets and variables → Actions** 中添加名为 `GITEE_TOKEN` 的 repository secret。不要将令牌写入 YAML、源码或提交信息。
3. 在 GitHub 创建仅选择 `lihechuan/EUI`、仅授权 **Actions: Read and write** 的 fine-grained token。在 Gitee Go 的流水线变量中添加加密变量 `EUI_GITHUB_DISPATCH_TOKEN`。该令牌仅用于请求运行固定的构建工作流。
4. 在 Gitee 仓库的 **流水线** 页面开通 Gitee Go，从代码视图导入 `.workflow/gitee-release.yml`。触发分支为 `master`，Python 构建命令为 `python packaging/gitee_release.py dispatch`。确认流水线变量会作为同名环境变量传入 Python 任务，然后保存并启用。
5. 按平台页面核对构建时长额度；需要付费时由仓库维护者处理。仅保存 YAML 文件不会自动完成流水线开通、变量配置和权限授权。

Gitee Go 提供触发时的 `GITEE_COMMIT` 和 `GITEE_BRANCH`，脚本同时读取该源码工作区的根目录 `CMakeLists.txt`，将完整提交 SHA 和版本交给 GitHub。GitHub 的 `GITEE_TOKEN` 只提供给源码下载和发行版发布步骤，不提供给实际编译步骤。

## 日常发布与失败重试

1. 更新根目录 `CMakeLists.txt` 的版本号、中英文下载入口、`CHANGELOG.md` 和 `packaging/release-notes.md`。
2. 将提交推送到 Gitee 的 `master`。Gitee Go 请求构建；版本对应的正式发行版及三个附件已经完整并通过下载校验时，Windows 打包步骤自动跳过。
3. 构建使用触发时的提交。若已有同版本标签但发行版未完成，使用该标签固定的提交恢复发布，避免将同一个版本重新指向其他源码。
4. 安装包、免安装 ZIP 和校验文件先发布到 Gitee 预发布版本；每个文件下载并核对 SHA-256 后，才转为正式发行版。Gitee API 未提供草稿字段，预发布阶段的附件可能已经可见。
5. 在 GitHub Actions 查看 **Gitee Windows release** 的构建和发布结果。Gitee Go 的触发步骤成功仅表示云端接受了请求，不表示附件已发布成功。
6. 发布步骤失败时，在原 GitHub Actions 运行中选择 **Re-run failed jobs**。发布作业会下载原构建附件，只补传缺失文件，不重建、不覆盖同名附件。已有同名文件与原包校验值不符时停止，交给维护者处理。

版本不变时仍会运行轻量检查，并下载已有附件核对 SHA-256；检查完整后不重新打包。工作流按版本串行执行，避免并发发布同一版本。构建产物超过 14 天失效后，需要维护者从原始可信构建恢复，不应覆盖已有附件。

GitHub 自身的版本发布流程继续由其现有工作流管理。Gitee 推送不会改写 GitHub 版本标签或已发布文件。需要双平台发布时，按 `CONTRIBUTING.md` 使用同一批已验证附件并分别检查下载；不能把两个独立构建的文件默认视为字节相同。

## 验证

```powershell
python -m unittest discover -s packaging -p test_gitee_release.py -v
python -m py_compile packaging/gitee_release.py
git diff --check
```

正式接入后，先手动运行一次 Gitee 流水线，核对 GitHub 请求、固定提交、两个 Windows 包、Gitee 三个附件及校验文件。然后以正常版本更新验证仅推送 Gitee 的完整发布链路。

接口与触发参数依据 [Gitee 官方 SDK 接口](https://gitee.com/sdk/gitee5j/blob/main/docs/RepositoriesApi.md)、[代码提交触发说明](https://help.gitee.com/enterprise/pipeline/source/trigger/)和[系统级变量说明](https://help.gitee.com/enterprise/pipeline/variable/system)。
