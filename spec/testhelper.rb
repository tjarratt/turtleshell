require 'turtleshell'

if GC.respond_to?(:verify_compation_references)
  GC.verify_compaction_references(double_heap: true, toward: :empty)
end

