void *
heap_alloc(heap *h, long size)
//void *BuddyPool::Acquire(size_t bytes)
{
    unsigned int kPrime = Log2Ceil(bytes + sizeof(Header));

    unsigned int i = kPrime;
                
    while (i <= m && sentinel[i].link.next == &sentinel[i]) {
    
        ++i;
        
    }
    
    if (i > m) {
    
        return NULL;    // throw bad_alloc("out of memory");
        
    }

    
    Block &block = *sentinel[i].link.next;
    
    Unlink(block);
    
    while (block.k > kPrime) {
    
        block.k -= 1;
        
        Block &buddy = Buddy(block);
        
        buddy.status = free;
        
        
        buddy.k = block.k;
        
        InsertAfter(sentinel[buddy.k], buddy);
        
    }
    
    block.status = reserved;
    
    return block.userPart;
}
