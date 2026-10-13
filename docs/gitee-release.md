# Gitee 版本自动发布 Windows 包

只需将版本更新推送到 Gitee 的 `master`，无需手工同步推送 GitHub 或上传附件。GitHub Actions 每约 15 分钟检查 Gitee 的版本号；发现尚未完成发布的版本后，Windows 云主机拉取固定的 Gitee 提交，运行现有打包和启动检查，最后上传安装包、免安装 ZIP 和 `SHA256SUMS.txt` 到 Gitee。

此方式不依赖 Gitee Go 的构建额度，也不需要部署触发服务。定时任务可能因 GitHub 调度负载延迟；公开仓库连续 60 天没有活动时，GitHub 会停用定时任务，需要在 Actions 页面重新启用。详见 [GitHub 定时触发说明](https://docs.github.com/en/actions/reference/workflows-and-actions/events-that-trigger-workflows#schedule)。

工作流固定发布仓库为 `xiaojiaz/EUI`，构建入口为 `lihechuan/EUI` 的 `gitee-release.yml`。它不会自动把 Gitee 源码推送到 GitHub 的公开分支；私有源码会在这个 GitHub 仓库的 Actions 主机上构建，Windows 包作为 Actions 构建产物保留 14 天。

## 一次性配置

1. 将 `.github/workflows/gitee-release.yml`、`packaging/gitee_release.py` 和 `packaging/test_gitee_release.py` 同步到 GitHub 的默认分支 `master`。
2. 在 Gitee 创建仓库级私人令牌，仅选择 `xiaojiaz/EUI`，授权 `projects`，用于读取源码和管理发行版。
3. 在 GitHub 仓库的 **Settings → Secrets and variables → Actions** 添加名为 `GITEE_TOKEN` 的 repository secret。不要将令牌写入 YAML、源码、提交信息或聊天。
4. 在 GitHub Actions 打开 **Gitee Windows release**，选择 **Run workflow** 验证一次。此后由定时任务自动检查，也可随时手动触发。

`GITEE_TOKEN` 只提供给版本检查、源码下载和发行版发布步骤，不提供给实际编译步骤。无需 `EUI_GITHUB_DISPATCH_TOKEN` 或 Gitee Go 流水线变量。

## 日常发布与失败重试

1. 更新根目录 `CMakeLists.txt` 的版本号、中英文下载入口、`CHANGELOG.md` 和 `packaging/release-notes.md`。
2. 将提交推送到 Gitee 的 `master`。定时检查将版本文件绑定到本次读取的完整提交 SHA；已有同版本正式发行版且三个附件齐全时跳过打包，普通提交不会重复发布。
3. 若已有同版本标签但发行版未完成，使用该标签固定的提交恢复发布，避免将同一个版本重新指向其他源码。
4. 安装包、免安装 ZIP 和校验文件先发布到 Gitee 预发布版本；每个文件下载并核对 SHA-256 后，才转为正式发行版。Gitee API 未提供草稿字段，预发布阶段的附件可能已经可见。
5. 在 GitHub Actions 查看 **Gitee Windows release** 的 `plan`、`package` 和 `publish` 结果。定时检查只读取元数据，上传后的完整性校验由发布步骤完成。
6. 发布步骤失败时，在原 GitHub Actions 运行中选择 **Re-run failed jobs**。发布作业会下载原构建附件，只补传缺失文件，不重建、不覆盖同名附件。已有同名文件与原包校验值不符时停止，交给维护者处理。

检查、构建和发布串行执行，避免同版本并发发布。构建产物超过 14 天失效后，需要维护者从原始可信构建恢复，不应覆盖已有附件。

GitHub 自身的版本发布流程继续由其现有工作流管理。Gitee 推送不会改写 GitHub 版本标签或已发布文件。需要双平台发布时，按 `CONTRIBUTING.md` 使用同一批已验证附件并分别检查下载；不能把两个独立构建的文件默认视为字节相同。

## 验证

```powershell
python -m unittest discover -s packaging -p test_gitee_release.py -v
python -m py_compile packaging/gitee_release.py
git diff --check
```

首次配置后，先验证已发布版本会跳过构建，再以正常版本更新检查固定提交、两个 Windows 包及 Gitee 下载校验。

接口依据 [Gitee 官方 SDK](https://gitee.com/sdk/gitee5j/blob/main/docs/RepositoriesApi.md)。
