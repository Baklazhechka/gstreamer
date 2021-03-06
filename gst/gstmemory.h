/* GStreamer
 * Copyright (C) 2009 Wim Taymans <wim.taymans@gmail.be>
 *
 * gstmemory.h: Header for memory blocks
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_MEMORY_H__
#define __GST_MEMORY_H__

#include <gst/gstconfig.h>

#include <glib-object.h>

G_BEGIN_DECLS

#define GST_TYPE_MEMORY (gst_memory_get_type())
GType gst_memory_get_type(void);

typedef struct _GstMemory GstMemory;
typedef struct _GstAllocator GstAllocator;

#define GST_MEMORY_CAST(mem)   ((GstMemory *)(mem))

/**
 * GstMemoryFlags:
 * @GST_MEMORY_FLAG_READONLY: memory is readonly. It is not allowed to map the
 * memory with #GST_MAP_WRITE.
 * @GST_MEMORY_FLAG_NO_SHARE: memory must not be shared. Copies will have to be
 * made when this memory needs to be shared between buffers.
 * @GST_MEMORY_FLAG_ZERO_PREFIXED: the memory prefix is filled with 0 bytes
 * @GST_MEMORY_FLAG_ZERO_PADDED: the memory padding is filled with 0 bytes
 * @GST_MEMORY_FLAG_LAST: first flag that can be used for custom purposes
 *
 * Flags for wrapped memory.
 */
typedef enum {
  GST_MEMORY_FLAG_READONLY      = GST_MINI_OBJECT_FLAG_LOCK_READONLY,
  GST_MEMORY_FLAG_NO_SHARE      = (GST_MINI_OBJECT_FLAG_LAST << 0),
  GST_MEMORY_FLAG_ZERO_PREFIXED = (GST_MINI_OBJECT_FLAG_LAST << 1),
  GST_MEMORY_FLAG_ZERO_PADDED   = (GST_MINI_OBJECT_FLAG_LAST << 2),

  GST_MEMORY_FLAG_LAST          = (GST_MINI_OBJECT_FLAG_LAST << 16)
} GstMemoryFlags;

/**
 * GST_MEMORY_FLAGS:
 * @mem: a #GstMemory.
 *
 * A flags word containing #GstMemoryFlags flags set on @mem
 */
#define GST_MEMORY_FLAGS(mem)  GST_MINI_OBJECT_FLAGS (mem)
/**
 * GST_MEMORY_FLAG_IS_SET:
 * @mem: a #GstMemory.
 * @flag: the #GstMemoryFlags to check.
 *
 * Gives the status of a specific flag on a @mem.
 */
#define GST_MEMORY_FLAG_IS_SET(mem,flag)   GST_MINI_OBJECT_FLAG_IS_SET (mem,flag)
/**
 * GST_MEMORY_FLAG_UNSET:
 * @mem: a #GstMemory.
 * @flag: the #GstMemoryFlags to clear.
 *
 * Clear a specific flag on a @mem.
 */
#define GST_MEMORY_FLAG_UNSET(mem,flag)   GST_MINI_OBJECT_FLAG_UNSET (mem, flag)

/**
 * GST_MEMORY_IS_READONLY:
 * @mem: a #GstMemory.
 *
 * Check if @mem is readonly.
 */
#define GST_MEMORY_IS_READONLY(mem)        GST_MEMORY_FLAG_IS_SET(mem,GST_MEMORY_FLAG_READONLY)
/**
 * GST_MEMORY_IS_NO_SHARE:
 * @mem: a #GstMemory.
 *
 * Check if @mem cannot be shared between buffers
 */
#define GST_MEMORY_IS_NO_SHARE(mem)        GST_MEMORY_FLAG_IS_SET(mem,GST_MEMORY_FLAG_NO_SHARE)
/**
 * GST_MEMORY_IS_ZERO_PREFIXED:
 * @mem: a #GstMemory.
 *
 * Check if the prefix in @mem is 0 filled.
 */
#define GST_MEMORY_IS_ZERO_PREFIXED(mem)   GST_MEMORY_FLAG_IS_SET(mem,GST_MEMORY_FLAG_ZERO_PREFIXED)
/**
 * GST_MEMORY_IS_ZERO_PADDED:
 * @mem: a #GstMemory.
 *
 * Check if the padding in @mem is 0 filled.
 */
#define GST_MEMORY_IS_ZERO_PADDED(mem)     GST_MEMORY_FLAG_IS_SET(mem,GST_MEMORY_FLAG_ZERO_PADDED)


/**
 * GstMemory:
 * @mini_object: parent structure
 * @allocator: pointer to the #GstAllocator
 * @parent: parent memory block
 * @maxsize: the maximum size allocated
 * @align: the alignment of the memory
 * @offset: the offset where valid data starts
 * @size: the size of valid data
 *
 * Base structure for memory implementations. Custom memory will put this structure
 * as the first member of their structure.
 */
struct _GstMemory {
  GstMiniObject   mini_object;

  GstAllocator   *allocator;

  GstMemory      *parent;
  gsize           maxsize;
  gsize           align;
  gsize           offset;
  gsize           size;
};

/**
 * GstMapFlags:
 * @GST_MAP_READ: map for read access
 * @GST_MAP_WRITE: map for write access
 * @GST_MAP_FLAG_LAST: first flag that can be used for custom purposes
 *
 * Flags used when mapping memory
 */
typedef enum {
  GST_MAP_READ      = GST_LOCK_FLAG_READ,
  GST_MAP_WRITE     = GST_LOCK_FLAG_WRITE,

  GST_MAP_FLAG_LAST = (1 << 16)
} GstMapFlags;

/**
 * GST_MAP_READWRITE:
 *
 * GstMapFlags value alias for GST_MAP_READ | GST_MAP_WRITE
 */
#define GST_MAP_READWRITE      (GST_MAP_READ | GST_MAP_WRITE)


/**
 * GstMapInfo:
 * @memory: a pointer to the mapped memory
 * @flags: flags used when mapping the memory
 * @data: (array length=size): a pointer to the mapped data
 * @size: the valid size in @data
 * @maxsize: the maximum bytes in @data
 * @user_data: extra private user_data that the implementation of the memory
 *             can use to store extra info.
 *
 * A structure containing the result of a map operation such as
 * gst_memory_map(). It contains the data and size.
 */
typedef struct {
  GstMemory *memory;
  GstMapFlags flags;
  guint8 *data;
  gsize size;
  gsize maxsize;
  /*< protected >*/
  gpointer user_data[4];

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
} GstMapInfo;

/**
 * GST_MAP_INFO_INIT:
 *
 * Initializer for #GstMapInfo
 */
#define GST_MAP_INFO_INIT { NULL, 0, NULL, 0, 0, }

/**
 * GstMemoryMapFunction:
 * @mem: a #GstMemory
 * @maxsize: size to map
 * @flags: access mode for the memory
 *
 * Get the memory of @mem that can be accessed according to the mode specified
 * in @flags. The function should return a pointer that contains at least
 * @maxsize bytes.
 *
 * Returns: a pointer to memory of which at least @maxsize bytes can be
 * accessed according to the access pattern in @flags.
 */
typedef gpointer    (*GstMemoryMapFunction)       (GstMemory *mem, gsize maxsize, GstMapFlags flags);

/**
 * GstMemoryUnmapFunction:
 * @mem: a #GstMemory
 *
 * Return the pointer previously retrieved with gst_memory_map().
 *
 * Returns: %TRUE on success.
 */
typedef void        (*GstMemoryUnmapFunction)     (GstMemory *mem);

/**
 * GstMemoryCopyFunction:
 * @mem: a #GstMemory
 * @offset: an offset
 * @size: a size or -1
 *
 * Copy @size bytes from @mem starting at @offset and return them wrapped in a
 * new GstMemory object.
 * If @size is set to -1, all bytes starting at @offset are copied.
 *
 * Returns: a new #GstMemory object wrapping a copy of the requested region in
 * @mem.
 */
typedef GstMemory * (*GstMemoryCopyFunction)      (GstMemory *mem, gssize offset, gssize size);

/**
 * GstMemoryShareFunction:
 * @mem: a #GstMemory
 * @offset: an offset
 * @size: a size or -1
 *
 * Share @size bytes from @mem starting at @offset and return them wrapped in a
 * new GstMemory object. If @size is set to -1, all bytes starting at @offset are
 * shared. This function does not make a copy of the bytes in @mem.
 *
 * Returns: a new #GstMemory object sharing the requested region in @mem.
 */
typedef GstMemory * (*GstMemoryShareFunction)     (GstMemory *mem, gssize offset, gssize size);

/**
 * GstMemoryIsSpanFunction:
 * @mem1: a #GstMemory
 * @mem2: a #GstMemory
 * @offset: a result offset
 *
 * Check if @mem1 and @mem2 occupy contiguous memory and return the offset of
 * @mem1 in the parent buffer in @offset.
 *
 * Returns: %TRUE if @mem1 and @mem2 are in contiguous memory.
 */
typedef gboolean    (*GstMemoryIsSpanFunction)    (GstMemory *mem1, GstMemory *mem2, gsize *offset);

void           gst_memory_init         (GstMemory *mem, GstMemoryFlags flags,
                                        GstAllocator *allocator, GstMemory *parent,
                                        gsize maxsize, gsize align,
                                        gsize offset, gsize size);
/* refcounting */
/**
 * gst_memory_ref:
 * @memory: The memory to refcount
 *
 * Increase the refcount of this memory.
 *
 * Returns: (transfer full): @memory (for convenience when doing assignments)
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC GstMemory * gst_memory_ref (GstMemory * memory);
#endif

static inline GstMemory *
gst_memory_ref (GstMemory * memory)
{
  return (GstMemory *) gst_mini_object_ref (GST_MINI_OBJECT_CAST (memory));
}

/**
 * gst_memory_unref:
 * @memory: (transfer full): the memory to refcount
 *
 * Decrease the refcount of an memory, freeing it if the refcount reaches 0.
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void gst_memory_unref (GstMemory * memory);
#endif

static inline void
gst_memory_unref (GstMemory * memory)
{
  gst_mini_object_unref (GST_MINI_OBJECT_CAST (memory));
}

/* getting/setting memory properties */
gsize          gst_memory_get_sizes    (GstMemory *mem, gsize *offset, gsize *maxsize);
void           gst_memory_resize       (GstMemory *mem, gssize offset, gsize size);

#define        gst_memory_lock(m,f)        gst_mini_object_lock (GST_MINI_OBJECT_CAST (m), (f))
#define        gst_memory_unlock(m,f)      gst_mini_object_unlock (GST_MINI_OBJECT_CAST (m), (f))
#define        gst_memory_is_writable(m)   gst_mini_object_is_writable (GST_MINI_OBJECT_CAST (m))
#define        gst_memory_make_writable(m) GST_MEMORY_CAST (gst_mini_object_make_writable (GST_MINI_OBJECT_CAST (m)))

/* retrieving data */
GstMemory *    gst_memory_make_mapped  (GstMemory *mem, GstMapInfo *info, GstMapFlags flags);
gboolean       gst_memory_map          (GstMemory *mem, GstMapInfo *info, GstMapFlags flags);
void           gst_memory_unmap        (GstMemory *mem, GstMapInfo *info);

/* copy and subregions */
GstMemory *    gst_memory_copy         (GstMemory *mem, gssize offset, gssize size);
GstMemory *    gst_memory_share        (GstMemory *mem, gssize offset, gssize size);

/* span memory */
gboolean       gst_memory_is_span      (GstMemory *mem1, GstMemory *mem2, gsize *offset);

G_END_DECLS

#endif /* __GST_MEMORY_H__ */
