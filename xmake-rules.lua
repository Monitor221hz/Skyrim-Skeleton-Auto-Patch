-- xmake-rules.lua
rule("template.copy_resources")
    after_install(function(target)
        local conf = target:extraconf("rules", "template.copy_resources")
        
        for _, entry in ipairs(conf.files or {}) do
            local src = path.join(os.projectdir(), entry.src)
            local dst = path.join(target:installdir(), entry.dest)
            
            if os.isdir(src) then
                os.mkdir(dst)
                os.trycp(path.join(src, "*"), dst, {recursive = true})
            elseif os.isfile(src) then
                os.mkdir(path.directory(dst))
                os.trycp(src, dst)
            end
        end
    end)